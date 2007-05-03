/* An actor that plays a sound from a file or URL.
 * 
 * Copyright (C) 2006 Jean-Sebastien Senecal (js@drone.ws)
 * Copyright (c) 2003-2006 The Regents of the University of California.
 * 
 * This file is a Frei0r plugin.
 * The code is a modified version of code from the Gimp.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
package drone.jmf.actor;

import java.io.IOException;

import javax.media.Controller;
import javax.media.ControllerEvent;
import javax.media.ControllerListener;
import javax.media.EndOfMediaEvent;
import javax.media.GainControl;
import javax.media.Manager;
import javax.media.MediaException;
import javax.media.Player;
import javax.media.Time;

import ptolemy.actor.TypedAtomicActor;
import ptolemy.actor.TypedIOPort;
import ptolemy.actor.parameters.IntRangeParameter;
import ptolemy.data.BooleanToken;
import ptolemy.data.DoubleToken;
import ptolemy.data.IntToken;
import ptolemy.data.expr.FileParameter;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

//////////////////////////////////////////////////////////////////////////
//// PlaySound

/**
 This actor plays audio from a file or URL when it fires.
 If the input has value <i>true</i>, then the sound is played.
 If it has value <i>false</i>, then the sound is stopped.
 If the input is not connected, or the actor fires with no input,
 then the sound is played when it fires.
 It requires the Java Media Framework.

 @author  Edward Lee
 @version $Id: PlaySound.java,v 1.29 2005/07/08 19:56:32 cxh Exp $
 @since Ptolemy II 3.0
 @Pt.ProposedRating Red (cxh)
 @Pt.AcceptedRating Red (cxh)
 */
public class PlaySound extends TypedAtomicActor implements ControllerListener {
    /** Construct an actor with the given container and name.
     *  @param container The container.
     *  @param name The name of this actor.
     *  @exception IllegalActionException If the actor cannot be contained
     *   by the proposed container.
     *  @exception NameDuplicationException If the container already has an
     *   actor with this name.
     */
    public PlaySound(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);

        fileNameOrURL = new FileParameter(this, "fileNameOrURL");

        synchronizedPlay = new Parameter(this, "synchronizedPlay");
        synchronizedPlay.setTypeEquals(BaseType.BOOLEAN);
        synchronizedPlay.setToken(BooleanToken.TRUE);

        onOff = new TypedIOPort(this, "onOff", true, false);
        onOff.setTypeEquals(BaseType.BOOLEAN);

        percentGain = new IntRangeParameter(this, "percentGain");

        // Set the default value to full scale.
        percentGain.setToken(new IntToken(100));
        
        period = new Parameter(this, "period");
        period.setTypeEquals(BaseType.DOUBLE);
        period.setExpression("1.0");
        
        phase = new Parameter(this, "phase");
        phase.setTypeEquals(BaseType.DOUBLE);
        phase.setExpression("0.0");
    }

    ///////////////////////////////////////////////////////////////////
    ////                     parameters and ports                  ////

    /** The file name or URL to read. */
    public FileParameter fileNameOrURL;

    /** The input port, which has type boolean.  A true input
     *  causes the sound to be played, and false input causes it
     *  to be stopped.
     */
    public TypedIOPort onOff;

    /** The gain (in percent).  This has as its value a record of the form
     *  {min = m, max = M, current = c}, where min <= c <= max.
     */
    public IntRangeParameter percentGain;

    /** Indicator to play to the end before returning from fire().
     *  This is a boolean, and defaults to true.
     */
    public Parameter synchronizedPlay;

    public Parameter period;
    
    public Parameter phase;

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    public void initialize() throws IllegalActionException {
    	_lastTime = -1.0;
    	super.initialize();
    }
    
    /** If the attribute is <i>fileNameOrURL</i>, then create a new
     *  player.
     *  @param attribute The attribute that changed.
     *  @exception IllegalActionException If the file cannot be opened
     *   or if the base class throws it.
     */
    public void attributeChanged(Attribute attribute)
            throws IllegalActionException {
        if (attribute == fileNameOrURL) {
            try {
                if ((fileNameOrURL != null) && (fileNameOrURL.asURL() != null)) {
                    if (_player != null) {
                        _player.removeControllerListener(this);
                    }

                    _player = Manager.createRealizedPlayer(fileNameOrURL
                            .asURL());
                    _player.addControllerListener(this);

                    // Initiate as much preprocessing as possible.
                    _player.prefetch();
                    _gainControl = _player.getGainControl();

                    if (percentGain != null) {
                        _gainControl.setLevel(0.01f * percentGain
                                .getCurrentValue());
                    }
                }
            } catch (IOException ex) {
                throw new IllegalActionException(this, "Cannot open file: "
                        + ex.toString());
            } catch (MediaException ex) {
                throw new IllegalActionException(this, ex,
                        "Exception thrown by media framework");
            }
        } else if ((attribute == percentGain) && (_gainControl != null)) {
            _gainControl.setLevel(0.01f * percentGain.getCurrentValue());
        } else if (attribute == synchronizedPlay) {
        	_synchronizedPlay = 
        		((BooleanToken) synchronizedPlay.getToken()).booleanValue();
            // Call this whether we have synchronized play or not, since
            // we may now have synchronized play but not have had it before.
        	if (_player != null)
                _player.stop();
        } else {
            super.attributeChanged(attribute);
        }
    }

    /** React to notification of a change in controller status.
     *  event The event.
     */
    public synchronized void controllerUpdate(ControllerEvent event) {
        System.out.println (event.getClass().getName());
        if (event instanceof EndOfMediaEvent) {
            // Specify that play should start at the beginning of the audio.
            _playSound = false;
            _player.setMediaTime(_startTime);
            _player.stop();
        }
        notifyAll();
    }

    /** Play the audio file.
     *  @exception IllegalActionException If there's no director.
     */
    public void fire() throws IllegalActionException {
        super.fire();

        // Consume the inputs.
        // Default if there is no input is to play the sound.
        _playSound = true; // big motherfucking hack

//        if ((onOff.getWidth() > 0) && onOff.hasToken(0)) {
//        	System.out.println("New input token.");
//            _playSound = ((BooleanToken) onOff.get(0)).booleanValue();
//            System.out.println("playsound = " + _playSound);
//        }
        
        boolean canPlay = false;
        double currentTime = getDirector().getModelTime().getDoubleValue();
//        System.out.println("Director: " + getDirector().getName() + ", currentime = " + currentTime);
        
        if ((_lastTime < 0.0 && currentTime >= ((DoubleToken)phase.getToken()).doubleValue()) ||
        	(_lastTime >= 0.0 && currentTime - _lastTime > ((DoubleToken)period.getToken()).doubleValue())) {
        		canPlay = true;
        		_lastTime = currentTime;
//        		System.out.println("can play");
        	}

        // If there is no player, then no sound file has been specified.
        // Just return.
        if (_player == null) {
            return;
        }

        
        if (_playSound && canPlay && _player.getState() != Controller.Started) {
//        	System.out.println("playing....");
        	_player.stop();
        	_playSound = false;
            _player.setMediaTime(_startTime);
            _player.start();

            // If synchronizedPlay is true, then wait for the play to complete.
            if (_synchronizedPlay) {
                synchronized (this) {
                    while ((_player.getState() == Controller.Started)
                            && !_stopRequested) {
                        try {
                            wait();
                        } catch (InterruptedException ex) {
                        	System.out.println("interrupted");
                            break;
                        }
                    }
                }
            }
        }
    }

    /** Override the base class to stop the currently playing audio.
     */
    public void stopFire() {
        super.stopFire();
//        System.out.println("stopfire");
//
//        if (_player != null) {
//            // FIXME: Doesn't seem to stop the sound.
//            _player.stop();
//        }
    }

    public void finalize() {
    	if (_player != null)
    		_player.stop();
    }
    
    /** Close the media processor.
     */
    public void wrapup() {
//    	System.out.println("wrapup");
//        if (_player != null) {
//            _player.stop();
//        }
    }

    ///////////////////////////////////////////////////////////////////
    ////                         private variables                 ////

    
    /** The gain control associated with the player. */
    private GainControl _gainControl;

    /** The player. */
    private Player _player;

    private boolean _synchronizedPlay = false;
    
    /** Start time for an audio clip. */
    private static Time _startTime = new Time(0.0);
    
    private double _lastTime;
    
    boolean _playSound = false;
}
