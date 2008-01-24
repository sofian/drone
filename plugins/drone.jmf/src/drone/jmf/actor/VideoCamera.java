/* An actor that produces a sequence of frames from a video camera.
 * 
 * Copyright (C) 2006 Jean-Sebastien Senecal (js@drone.ws)
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

/* Some of this code is copied from Sun's sample code.  Hence:
 *
 * @(#)FrameAccess.java        1.5 01/03/13
 *
 * Copyright (c) 2002-2006 Sun Microsystems, Inc. All Rights Reserved.
 *
 * Sun grants you ("Licensee") a non-exclusive, royalty free, license to use,
 * modify and redistribute this software in source and binary code form,
 * provided that i) this copyright notice and license appear on all copies of
 * the software; and ii) Licensee does not utilize the software in a manner
 * which is disparaging to Sun.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY
 * IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE
 * LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR ITS
 * LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT,
 * INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF
 * OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 * This software is not designed or intended for use in on-line control of
 * aircraft, air traffic, aircraft navigation or aircraft communications; or in
 * the design, construction, operation or maintenance of any nuclear
 * facility. Licensee represents and warrants that it will not use or
 * redistribute the Software for such purposes.
 */
package drone.jmf.actor;

import drone.jmf.data.JMFImageToken;

import java.awt.Dimension;
import java.util.Iterator;
import java.util.Vector;

import javax.media.Buffer;
import javax.media.CaptureDeviceInfo;
import javax.media.CaptureDeviceManager;
import javax.media.Codec;
import javax.media.ConfigureCompleteEvent;
import javax.media.Controller;
import javax.media.ControllerClosedEvent;
import javax.media.ControllerEvent;
import javax.media.ControllerListener;
import javax.media.EndOfMediaEvent;
import javax.media.Format;
import javax.media.Manager;
import javax.media.MediaLocator;
import javax.media.NoDataSourceException;
import javax.media.NoPlayerException;
import javax.media.Player;
import javax.media.PrefetchCompleteEvent;
import javax.media.Processor;
import javax.media.RealizeCompleteEvent;
import javax.media.ResourceUnavailableEvent;
import javax.media.StartEvent;
import javax.media.StopEvent;
import javax.media.UnsupportedPlugInException;
import javax.media.control.FormatControl;
import javax.media.control.FrameGrabbingControl;
import javax.media.control.TrackControl;
import javax.media.format.RGBFormat;
import javax.media.format.VideoFormat;
import javax.media.format.YUVFormat;
import javax.media.protocol.CaptureDevice;
import javax.media.protocol.DataSource;

import ptolemy.actor.lib.Source;
import ptolemy.data.IntToken;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.StringAttribute;

//////////////////////////////////////////////////////////////////////////
//// VideoCamera

/**
 An actor that produces a sequence of frames from a video camera.
 This actor requires the Java Media Framework, and has been tested
 at least with version 2.1.1.

 FIXME: more info.

 @author  Christopher Hylands, Edward Lee, James Yeh, Paul Yang, David Lee
 @version $Id: VideoCamera.java,v 1.42 2006/08/21 23:11:50 cxh Exp $
 @since Ptolemy II 3.0
 @Pt.ProposedRating Red
 @Pt.AcceptedRating Red
 */
public class VideoCamera extends Source implements ControllerListener {

	public static final String FRAME_GRABBING_CONTROL_NAME = "javax.media.control.FrameGrabbingControl";

    /** Construct an actor with the given container and name.
     *  @param container The container.
     *  @param name The name of this actor.
     *  @exception IllegalActionException If the actor cannot be contained
     *   by the proposed container.
     *  @exception NameDuplicationException If the container already has an
     *   actor with this name.
     */
    public VideoCamera(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);

        formatName = new StringAttribute(this, "formatName");
        formatName.setExpression("RGB");

        formatNumber = new Parameter(this, "formatNumber");
        formatNumber.setExpression("0");
        
        deviceNumber = new Parameter(this, "deviceNumber");
        deviceNumber.setExpression("0");

        // FIXME: output should perhaps be named "video"?
        // In case there is audio track.
        // Don't derive from source in this case.
        output.setTypeEquals(BaseType.OBJECT);
    }

    ///////////////////////////////////////////////////////////////////
    ////                     ports and parameters                  ////

    /** The type of video format to use.  This is a string valued
     *  attribute that defaults to the type RGB.
     */
    public StringAttribute formatName;
    
    /**
     * Index to use for format in the format list.
     * 
     * TODO: find a better way to handle formats
     */
    public Parameter formatNumber;

    /** This parameter lets the user select the device to use.
     *  Typically this parameter is of no concern and should be left
     *  at 0.  However, if a computer has more than one usable
     *  cameras, this parameter can be used to choose amongst them.
     */
    public Parameter deviceNumber;

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////
    
    /** The controller listener.  This method controls the
     *  initializing of the player.  It also senses when the
     *  file is done playing, in which case it closes the
     *  player.
     *  @param event The controller event.
     */
    public void controllerUpdate(ControllerEvent event) {
        if (event instanceof ConfigureCompleteEvent
                || event instanceof RealizeCompleteEvent
                || event instanceof PrefetchCompleteEvent
                || event instanceof StartEvent
                || event instanceof StopEvent) {
            synchronized (_waitSync) {
                _stateTransitionEvent = event;
                _stateTransitionOK = true;
                _waitSync.notifyAll();
            }
        } else if (event instanceof ControllerClosedEvent) {
            _stateTransitionEvent = event;
            _player.close();
            _playerOpen = false;
        }
        // TODO: verify if there are other events to catch
    }

    public boolean prefire() throws IllegalActionException {
        if (_playerOpen == false) {
            _dataSource.disconnect();
            return false;
        } else {
        	_bufferNew = _frameGrabbingControl.grabFrame();
        	return super.prefire();
        }
    }
    
    /** Capture a frame and send a java.awt.Image object
     *  to the output port.
     *  @exception IllegalActionException If there's no director.
     */
    public void fire() throws IllegalActionException {
        super.fire();
        if (_bufferNew != null) {
        	// If trigger is connected, then it must send a token to activate output.
        	if (trigger.getWidth() <= 0 || trigger.hasToken(0)) {
        		output.send(0, new JMFImageToken(_bufferNew));
        	}
        }
    }

    /** Open the file at the URL, and set the width of the output.
     *  @exception IllegalActionException If there are no video capture
     *   devices.
     */
    public void initialize() throws IllegalActionException {
        super.initialize();

        try {
        	Class.forName("javax.media.Codec");
        } catch (Exception e) {
        	e.printStackTrace();
        }
        // Set the video format type given which setting
        // the formatName parameter is set to.
        String typeName = formatName.getExpression();

        if (typeName.equals("YUV")) {
            _format = new YUVFormat();
        } else if (typeName.equals("RGB")) {
            _format = new RGBFormat();
        } else {
            throw new IllegalActionException(this,
                    "Unrecognized interpolation type: " + typeName);
        }
        
        // Get the list of devices that are compatible with the
        // chosen video format.
        // FIXME: Devicelist should be a static private member
        Vector deviceList = CaptureDeviceManager.getDeviceList(_format);

        // If the list of devices is empty, then we must throw
        // an exception.
        if (deviceList.size() == 0) {
            throw new IllegalActionException(this,
                    "No video capture devices found by the "
                            + "Java Media Framework.");
        }

        // List the devices in the debug window.
        if (_debugging) {
            _debug("--- Capture devices found:");

            Iterator devices = deviceList.iterator();

            while (devices.hasNext()) {
                CaptureDeviceInfo device = (CaptureDeviceInfo) devices.next();
                _debug(device.getName());
            }

            _debug("---");
        }

        // Choose the device from the device list.
        // FIXME: This isn't crashing gracefully at all.
        CaptureDeviceInfo captureDeviceInfo = (CaptureDeviceInfo) deviceList
        	.get(((IntToken) deviceNumber.getToken()).intValue());

        System.out.println("-----------------------------------------------------------");
        System.out.println("CaptureDevice Name is " + captureDeviceInfo.getName());
        System.out.println("-----------------------------------------------------------");
        System.out.println("Media Locator is " + captureDeviceInfo.getLocator() );
        System.out.println("***********************************************************");
        try {
        	_dataSource = Manager.createDataSource(captureDeviceInfo.getLocator());
            FormatControl [] formatControlArray = ((CaptureDevice) _dataSource).getFormatControls();
            if (formatControlArray != null && formatControlArray.length > 0) {
                System.out.println("Found " + formatControlArray.length + " FormatControls");
                Format[] formats = formatControlArray[0].getSupportedFormats();
                System.out.println("Found " + formats.length + " supported formats from format control # 0");
                for (int i=0; i<formats.length; i++) {
                	System.out.println("[" + i + "] = " + formats[i].toString());
                }
                int i = ((IntToken)formatNumber.getToken()).intValue();
                if (formats[i].matches(_format)) {
                	formatControlArray[0].setFormat(formats[i]);
            		System.out.println("Format #" + i + " matches");
                }
                else {
                	throw new IllegalActionException(this, "Chosen format number does not match chosen format.");
                }
//                if ( formatControlArray[0].setFormat(format) == null ) {
//                    System.out.println("Failed to set Format " + format);
//                    System.exit(0);
//                }
            } else {
            	throw new IllegalActionException(this, "Failed to find FormatControls");
            }
            // Create player
            _player = Manager.createPlayer(_dataSource);
        } catch (NoDataSourceException e) {
        	throw new IllegalActionException(this, e, "No data source found.");
        } catch (java.io.IOException e) {
        	throw new IllegalActionException(this, e, "I/O problem.");
        } catch (NoPlayerException e) {
        	throw new IllegalActionException(this, e, "Cannot create player.");
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
        
        _frameGrabbingControl = (FrameGrabbingControl) _player.getControl(FRAME_GRABBING_CONTROL_NAME);

        if (_frameGrabbingControl == null) {
        	System.out.println(
        			"Failed to get Frame Grabbing Control '"
        			+ FRAME_GRABBING_CONTROL_NAME );       
        }

        _player.prefetch();

        if (!_waitForState(Controller.Prefetched)) {
            throw new IllegalActionException(this,
                    "Failed to prefetch player, last controller event was: "
                            + _stateTransitionEvent + "\nThe data source was: "
                            + _dataSource.getLocator().toExternalForm());
        }
        
        _player.start();
        System.out.println("Called start on player");
        
        if (!_waitForState(Controller.Started)) {
            throw new IllegalActionException(this,
                    "Failed to prefetch player, last controller event was: "
                            + _stateTransitionEvent + "\nThe data source was: "
                            + _dataSource.getLocator().toExternalForm());
        }
        
        _playerOpen = true;
        
        System.out.println("Player started");
    	_bufferNew = _frameGrabbingControl.grabFrame();
    }

    /** Close the media processor.
     */
    public void wrapup() {
        if (_player != null) {
            _player.close();
        }
    }

    ///////////////////////////////////////////////////////////////////
    ////                         protected methods                 ////

    /** Block until the processor has transitioned to the given state.
     *  Return false if the transition failed.
     *  @param state The state for which we are waiting.
     *  @return True if the state transition is ok, otherwise return false.
     */
    protected boolean _waitForState(int state) {
        synchronized (_waitSync) {
            try {
                while ((_player.getState() != state) && _stateTransitionOK) {
                    _waitSync.wait();
                }
            } catch (Exception e) {
            }
        }

        return _stateTransitionOK;
    }

    protected void createDataSource() {
    	
    }
    
    ///////////////////////////////////////////////////////////////////
    ////                         private variables                 ////
    // The datasource that encapsulates the video file.
    private DataSource _dataSource;

    private VideoFormat _format;

    /** The java.awt.Image that we are producing/ */
    private Buffer _bufferNew;

    // The player.
    private Player _player;

    // Boolean that keeps track of whether the player is open or not.
    private boolean _playerOpen = false;

   private Object _waitSync = new Object();

    private boolean _stateTransitionOK = true;

    // The Frame grabbing control class that allows individual frames
    // to be acquired from the file.
    private FrameGrabbingControl _frameGrabbingControl = null;
    
    // The ControllerEvent that was present when _stateTransitionOK
    // was last set;
    private ControllerEvent _stateTransitionEvent;

}
