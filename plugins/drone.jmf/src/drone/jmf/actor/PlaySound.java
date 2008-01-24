/* An actor that plays a sound from a file or URL.
 * 
 * Copyright (C) 2006-2007 Jean-Sebastien Senecal (js@drone.ws)
 * Copyright (c) 2003-2006 The Regents of the University of California.
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
import java.net.URL;

import javax.media.CannotRealizeException;
import javax.media.Controller;
import javax.media.ControllerEvent;
import javax.media.ControllerListener;
import javax.media.EndOfMediaEvent;
import javax.media.GainControl;
import javax.media.Manager;
import javax.media.MediaException;
import javax.media.MediaLocator;
import javax.media.NoPlayerException;
import javax.media.Player;
import javax.media.Time;

import ptolemy.actor.TypedAtomicActor;
import ptolemy.actor.TypedIOPort;
import ptolemy.actor.parameters.DoubleRangeParameter;
import ptolemy.data.BooleanToken;
import ptolemy.data.DoubleToken;
import ptolemy.data.expr.FileParameter;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

import drone.util.MathUtils;

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

	/**
	 *  Construct an actor with the given container and name.
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

		onOff = new TypedIOPort(this, "onOff", true, false);
		onOff.setTypeEquals(BaseType.BOOLEAN);

		gain = new TypedIOPort(this, "gain", true, false);
		gain.setTypeEquals(BaseType.DOUBLE);

		fileNameOrURL = new FileParameter(this, "fileNameOrURL");

		loop = new Parameter(this, "loop");
		loop.setTypeEquals(BaseType.BOOLEAN);
		loop.setToken(BooleanToken.TRUE);

		defaultGain = new DoubleRangeParameter(this, "defaultGain");
		defaultGain.setToken(new DoubleToken(1.0)); // Set the default value to full scale.
	}

	///////////////////////////////////////////////////////////////////
	////                     parameters and ports                  ////

	/** 
	 * The file name or URL to read. 
	 */
	public FileParameter fileNameOrURL;

	/** 
	 * The input port, which has type boolean.  A true input
	 *  causes the sound to be played, and false input causes it
	 *  to be stopped.
	 */
	public TypedIOPort onOff;

	/** 
	 * The gain (as a value between 0 and 1).  This has as its value a record of the form
	 *  {min = m, max = M, current = c}, where min <= c <= max.
	 */
	public TypedIOPort gain;

	/** 
	 * The default gain.
	 */
	public DoubleRangeParameter defaultGain;

	/**
	 * Boolean parameter telling wether the player should play the file in loop or not.
	 */
	public Parameter loop;

	///////////////////////////////////////////////////////////////////
	////                         public methods                    ////

	public void initialize() throws IllegalActionException {
		_isOn = true;
		try {
			_createNewPlayer(_audioFileURL);
		} catch (IOException ex) {
			throw new IllegalActionException(this, ex,
					"Cannot open file/url: " + fileNameOrURL.asURL().toString());
		} catch (MediaException ex) {
			throw new IllegalActionException(this, ex,
			"Exception thrown by media framework");
		}		
		super.initialize();
	}

	/** If the attribute is <i>fileNameOrURL</i>, then create a new
	 *  player.
	 *  @param attribute The attribute that changed.
	 *  @exception IllegalActionException If the file cannot be opened
	 *   or if the base class throws it.
	 */
	public void attributeChanged(Attribute attribute) throws IllegalActionException {
		if (attribute == fileNameOrURL) {
			if ((fileNameOrURL != null) && (fileNameOrURL.asURL() != null)) {
				_audioFileURL = fileNameOrURL.asURL();
			}
		} else if (attribute == loop) {
			_isLooping = ((BooleanToken) loop.getToken()).booleanValue();
		} else {
			super.attributeChanged(attribute);
		}
	}

	/** Play the audio file. This needs to be called only once every time.
	 *  @exception IllegalActionException If there's no director.
	 */
	public boolean postfire() throws IllegalActionException {

		// Change on/off if specified in input.
		if (onOff.getWidth() > 0 && onOff.hasToken(0)) {
			_isOn = ((BooleanToken)onOff.get(0)).booleanValue();
		}

		if (_player != null) {

			// Set gain.
			if (gain.getWidth() > 0 && gain.hasToken(0)) {
				float gainLevel = (float) ((DoubleToken)gain.get(0)).doubleValue();
				_gainControl.setLevel(MathUtils.clamp(gainLevel, 0.0f, 1.0f));
			}

			if (!_isOn) {
				if (_player.getState() == Controller.Started)
					_player.stop();
			} else {
				if (_player.getState() != Controller.Started)
					_player.start();

			}
		}
		
		return true;
	}

	/** Override the base class to stop the currently playing audio.
	 */
	public void stopFire() {
		super.stopFire();
	}

	/** 
	 * Close the media processor and deallocate all resources.
	 */
	public void wrapup() {
		_terminatePlayer();
	}

	/** React to notification of a change in controller status.
	 *  event The event.
	 */
	public synchronized void controllerUpdate(ControllerEvent event) {
		if (event instanceof EndOfMediaEvent) {
			// Specify that play should start at the beginning of the audio.
			if (_player != null) {
				// Rewind.
				_player.setMediaTime(_startTime);

				// Stop if not looping.
				if (!_isLooping) {
					_player.stop();
					_isOn = false;
				}
			}
		}
		notifyAll();
	}

	/**
	 * Creates the new player and realizes it.
	 * @param url the URL pointing to the media file to play
	 * @throws NoPlayerException
	 * @throws IOException
	 * @throws IllegalActionException
	 * @throws CannotRealizeException
	 */
	protected void _createNewPlayer(URL url) throws NoPlayerException, IOException, IllegalActionException, CannotRealizeException {

		// Set media location.
		MediaLocator source = new MediaLocator(url);
		if (_player != null) {
			_player.removeControllerListener(this);
			_terminatePlayer();
		}
		
		if (source == null) {
			throw new IllegalActionException("Cannot create a source out of given URL '" + url.toString() + "'");
		}
		
		// Create the player.
		_player = Manager.createRealizedPlayer(source);

		// Initialize.
		_player.addControllerListener(this);

		// Set gain control.
		_gainControl = _player.getGainControl();
		_gainControl.setLevel((float) MathUtils.clamp(((DoubleToken)defaultGain.getToken()).doubleValue(), 0.0, 1.0));
	}

	/**
	 * Terminates the player and sets it to null.
	 */
	protected void _terminatePlayer()  {
		if (_player != null) {
			_player.stop();
			_player.close();
			_player.deallocate();
			_player = null;
		}
	}

	///////////////////////////////////////////////////////////////////
	////                         private variables                 ////


	/** The gain control associated with the player. */
	private GainControl _gainControl = null;

	/** The player. */
	private Player _player = null;

	/** Start time for an audio clip. */
	private static Time _startTime = new Time(0.0);

	private boolean _isOn = true;

	private boolean _isLooping = true;

	private URL _audioFileURL = null;
}