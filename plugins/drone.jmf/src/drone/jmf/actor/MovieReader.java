/* An actor that outputs frames from a video file.
 * 
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

import java.io.File;
import java.net.URL;

import javax.media.Buffer;
import javax.media.ConfigureCompleteEvent;
import javax.media.Control;
import javax.media.Controller;
import javax.media.ControllerEvent;
import javax.media.ControllerListener;
import javax.media.EndOfMediaEvent;
import javax.media.Manager;
import javax.media.Player;
import javax.media.PrefetchCompleteEvent;
import javax.media.RealizeCompleteEvent;
import javax.media.ResourceUnavailableEvent;
import javax.media.Time;
import javax.media.control.FrameGrabbingControl;
import javax.media.control.FramePositioningControl;
import javax.media.protocol.DataSource;

import ptolemy.util.JNLPUtilities;
import ptolemy.actor.TypedIOPort;
import ptolemy.actor.lib.Source;
import ptolemy.data.BooleanToken;
import ptolemy.data.IntToken;
import ptolemy.data.ScalarToken;
import ptolemy.data.expr.FileParameter;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.KernelException;
import ptolemy.kernel.util.NameDuplicationException;

import drone.jmf.data.JMFImageToken;

//////////////////////////////////////////////////////////////////////////
//// MovieReader

/**
 This actor loads a video file (MPEG, AVI, or Quicktime files only), and
 outputs each frame as a JMFImageToken.

 <p>Note that not all formats are supported, for details, see the JMF faq at:
 <a href="http://java.sun.com/products/java-media/jmf/reference/faqs/index.html#in_browser" target="_top">http://java.sun.com/products/java-media/jmf/reference/faqs/index.html</a>

 @author James Yeh
 @version $Id: MovieReader.java,v 1.31 2006/08/21 23:11:47 cxh Exp $
 @since Ptolemy II 4.0
 @Pt.ProposedRating Red (cxh)
 @Pt.AcceptedRating Red (cxh)
 */
public class MovieReader extends Source implements ControllerListener {

    /** Construct an actor with the given container and name.
     *  @param container The container.
     *  @param name The name of this actor.
     *  @exception IllegalActionException If the actor cannot be contained
     *   by the proposed container.
     *  @exception NameDuplicationException If the container already has an
     *   actor with this name.
     */
    public MovieReader(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);
        output.setTypeEquals(BaseType.OBJECT);
        fileOrURL = new FileParameter(this, "fileOrURL");
        // TODO: change MrPtolemy.mov to a more "dronesque" movie
        fileOrURL.setExpression("$CLASSPATH/plugins/drone.jmf/data/MrPtolemy.mov");
        isFrameBased = new Parameter(this, "isFrameBased");
        isFrameBased.setTypeEquals(BaseType.BOOLEAN);
        isFrameBased.setExpression("true");
    }

    ///////////////////////////////////////////////////////////////////
    ////                     ports and parameters                  ////

    /** The file name or URL from which to read.  This is a string with
     *  any form accepted by File Attribute.  The default value is "".
     *  @see FileParameter
     */
    public FileParameter fileOrURL;
    
    public Parameter isFrameBased;

//    // Input for either frame number or time. Default is -1.
//    public TypedIOPort frameTimeInput;
//    
//    // Image output.
//    public TypedIOPort imageOutput;

    public static final String FRAME_POSITIONING_CONTROL_NAME = "javax.media.control.FramePositioningControl";
    public static final String FRAME_GRABBING_CONTROL_NAME = "javax.media.control.FrameGrabbingControl";
    
    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** An attempt is made to acquire the file name.  If it is
     *  successful, create the DataSource that encapsulates the file.
     *  @param attribute The attribute that changed.
     *  @exception IllegalActionException If the URL is null, or
     *  invalid.
     */
    public void attributeChanged(Attribute attribute)
            throws IllegalActionException {
    	// FIXME: if the attribute fileOrURL is invalid, simply output a default (black? blue?) screen
        if (attribute == fileOrURL) {
            URL url = fileOrURL.asURL();

            if (url == null) {
                throw new IllegalActionException("URLToken was null");
            } else {
                try {
                    _dataSource = Manager.createDataSource(url);
                } catch (Exception ex) {
                    URL urlCopy = null;
                    String copyFileName = "";

                    try {
                        // Web Start: javax.media.Manager.createDataSource()
                        // does not deal with jar urls because it cannot
                        // find a data source, so we copy the jar file.
                        copyFileName = JNLPUtilities.saveJarURLAsTempFile(url
                                .toString(), "JMFMovieReader", null, null);

                        File copyFile = new File(copyFileName);
                        urlCopy = copyFile.toURL();
                        _dataSource = Manager.createDataSource(urlCopy);
                    } catch (Exception ex2) {
                        // Ignore this exception, throw the original.
                        throw new IllegalActionException(this, ex,
                                "Invalid URL.\n(Tried copying the file '"
                                        + url.toString()
                                        + "', to '"
                                        + ((urlCopy == null) ? "null" : urlCopy
                                                .toString())
                                        + "', (copyFileName was: '"
                                        + copyFileName
                                        + "') but that failed with:\n"
                                        + KernelException
                                                .stackTraceToString(ex2));
                    }
                }
            }
        } else if (attribute == isFrameBased) {
        	try {
        		_isFrameBased = ((BooleanToken)isFrameBased.getToken()).booleanValue();
        	} catch (Exception ex) {
        		throw new IllegalActionException(this, ex, "Parameter isFrameBased is invalid.");
        	}
        } else {
            super.attributeChanged(attribute);
        }
    }

    /** The controller listener.  This method controls the
     *  initializing of the player.  It also senses when the
     *  file is done playing, in which case it closes the
     *  player.
     *  @param event The controller event.
     */
    public void controllerUpdate(ControllerEvent event) {
        if (event instanceof ConfigureCompleteEvent
                || event instanceof RealizeCompleteEvent
                || event instanceof PrefetchCompleteEvent) {
            synchronized (_waitSync) {
                _stateTransitionEvent = event;
                _stateTransitionOK = true;
                _waitSync.notifyAll();
            }
        } else if (event instanceof ResourceUnavailableEvent) {
            synchronized (_waitSync) {
                _stateTransitionEvent = event;
                _stateTransitionOK = false;
                _waitSync.notifyAll();
            }
        } else if (event instanceof EndOfMediaEvent) {
            _stateTransitionEvent = event;
            _player.close();
            _playerOpen = false;
        }
    }

    /** Send a JMFImageToken out through the output port.
     *  @exception IllegalActionException If there's no director.
     */
    public void fire() throws IllegalActionException {
    	
        // NOTE: It might seem that using trigger.numberOfSources() is
        // correct here, but it is not. It is possible for channels
        // to be connected, for example, to other output ports or
        // even back to this same trigger port, in which case higher
        // numbered channels will not have their inputs read.
    	if (trigger.getWidth() > 0) {
    		for (int i = 0; i < trigger.getWidth(); i++) {
    			if (trigger.hasToken(i)) {
    				try {
    					_updateFrameTime((ScalarToken)trigger.get(i));
    				} catch (Exception ex) {
    					throw new IllegalActionException(this, ex, "Error processing trigger token " + i);
    				}
    				output.send(0, new JMFImageToken(_frame));
    			}
    		}
    	} else {
    		_updateFrameTime(new IntToken(-1));
    		output.send(0, new JMFImageToken(_frame));
    	}
    	super.fire();
    }

    /** An attempt is made to acquire both the frame grabbing and
     *  frame positioning controls.  If both succeed, the first
     *  frame is acquired.
     *  @exception IllegalActionException If the either frame
     *  grabbing control or frame positioning control cannot
     *  be acquired, or if a contained method throws it.
     */
    public void initialize() throws IllegalActionException {
        super.initialize();

        try {
            _player = Manager.createPlayer(_dataSource);
        } catch (Exception ex) {
            throw new IllegalActionException(
                    this,
                    ex,
                    "Failed to create a player for the data source. "
                            + "Note that you may need to run jmfinit, which is found "
                            + "in the JMF directory, for example c:/Program Files/"
                            + "JMF2.1.1/bin.  The data source was: "
                            + _dataSource.getLocator().toExternalForm());
        }

        _player.addControllerListener(this);

        _player.realize();

        if (!_waitForState(Controller.Realized)) {
            throw new IllegalActionException(
                    null,
                    "Failed to realize player, last controller event was: "
                            + _stateTransitionEvent
                            + "\nThe data source was: "
                            + _dataSource.getLocator().toExternalForm()
                            + "\nNote that not all formats are supported, see:\n"
                            + "http://java.sun.com/products/java-media/jmf/reference/faqs/index.html");
        }

        _framePositioningControl = (FramePositioningControl) _player
        .getControl(FRAME_POSITIONING_CONTROL_NAME);

        if (_framePositioningControl == null) {
        	throw new IllegalActionException(this,
        			"Failed to get Frame Positioning Control '"
        			+ FRAME_POSITIONING_CONTROL_NAME
        			+ "' possible controls are:\n" + _controlNames()
        			+ "\nThe data source was: "
        			+ _dataSource.getLocator().toExternalForm());
        }
        
        _frameGrabbingControl = (FrameGrabbingControl) _player
        .getControl(FRAME_GRABBING_CONTROL_NAME);

        if (_frameGrabbingControl == null) {
        	throw new IllegalActionException(this,
        			"Failed to get Frame Grabbing Control '"
        			+ FRAME_GRABBING_CONTROL_NAME
        			+ "' possible controls are:\n" + _controlNames()
        			+ "\nThe data source was: "
        			+ _dataSource.getLocator().toExternalForm());
        }
        
        _player.prefetch();

        if (!_waitForState(Controller.Prefetched)) {
            throw new IllegalActionException(this,
                    "Failed to prefetch player, last controller event was: "
                            + _stateTransitionEvent + "\nThe data source was: "
                            + _dataSource.getLocator().toExternalForm());
        }
        
        _framePositioningControl.skip(1);
        _frame = _frameGrabbingControl.grabFrame();
    }

    /** If the player is no longer open, then disconnect the
     *  datasource.  If the player is still open, acquire the next
     *  frame.
     *  @return false if the player is no longer open, otherwise
     *  return super.postfire().
     *  @exception IllegalActionException If thrown by the super class.
     */
    public boolean postfire() throws IllegalActionException {
    	return super.postfire();
    }

    ///////////////////////////////////////////////////////////////////
    ////                         protected methods                 ////

    /** Block until the processor has transitioned to the given state.
     *  @param state The state.
     *  @return false if the transition failed.
     *  @exception IllegalActionException If there is a problem blocking
     *  the processor until the state transition is completed.
     */
    protected boolean _waitForState(int state) throws IllegalActionException {
        synchronized (_waitSync) {
            try {
                while ((_player.getState() != state) && _stateTransitionOK) {
                    _waitSync.wait();
                }
            } catch (Exception ex) {
                throw new IllegalActionException(this, ex,
                        "Failed block the processor until it state"
                                + " transition completed.");
            }
        }

        return _stateTransitionOK;
    }

    protected boolean _updateFrameTime(ScalarToken inputToken) throws IllegalActionException {
        if (_playerOpen == false) {
            _dataSource.disconnect();
            return false;
        } else {
        	try {
        		if (_isFrameBased) {
            		// frame based
            		int frameNo = inputToken.intValue();
            		if (frameNo < 0)
            			// default: skip to next frame
            			_framePositioningControl.skip(1);
            		else
            			_framePositioningControl.seek(frameNo);
            	} else {
            		// time based
            		double mediaTimeDouble = inputToken.doubleValue();
            		Time mediaTime;
            		if (mediaTimeDouble < 0) {
            			mediaTime = new Time(getDirector().getModelTime().getDoubleValue());
            		} else {
            			mediaTime = new Time(mediaTimeDouble);
            		}
            		// FIXME: if mediaTimeDouble < 0 there is a much more efficient way to
            		// do this by skipping a number of frames
            		_framePositioningControl.seek(_framePositioningControl.mapTimeToFrame(mediaTime));
            	}
        		_frame = _frameGrabbingControl.grabFrame();
        	} catch (Exception ex) {
        		throw new IllegalActionException(this, ex, "Problem in call to postfire().");
        	}
            return true;
        }
    }
    
    // Return a string containing the names of the possible controls
    private String _controlNames() {
        StringBuffer controlNames = new StringBuffer();

        try {
            Control[] controls = _player.getControls();

            for (int i = 0; i < controls.length; i++) {
                controlNames.append(controls[i] + "\n");
            }
        } catch (Exception ex) {
            controlNames.append("Could not get controls: " + ex);
        }

        if (controlNames.toString().length() == 0) {
            controlNames.append("None");
        }

        return controlNames.toString();
    }

    ///////////////////////////////////////////////////////////////////
    ////                         private variables                 ////
    // The datasource that encapsulates the video file.
    private DataSource _dataSource;

    // The individual frame to be sent to the output port.
    private Buffer _frame;

    // The Frame grabbing control class that allows individual frames
    // to be acquired from the file.
    private FrameGrabbingControl _frameGrabbingControl = null;

    // The Frame positioning control class that allows control over
    // which frame is the current one.
    private FramePositioningControl _framePositioningControl;

    // The player.
    private Player _player;

    // Boolean that keeps track of whether the player is open or not.
    private boolean _playerOpen = true;

    // The ControllerEvent that was present when _stateTransitionOK
    // was last set;
    private ControllerEvent _stateTransitionEvent;

    // Boolean that keeps track of whether the player initialization
    // has gone through smoothly.
    private boolean _stateTransitionOK = true;

    // Object to allow synchronization in this actor.
    private Object _waitSync = new Object();
    
    // This is the boolean corresponding to the isFrameBased parameter.
    private boolean _isFrameBased = true;
}
