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

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import com.lti.civil.CaptureDeviceInfo;
import com.lti.civil.CaptureException;
import com.lti.civil.CaptureObserver;
import com.lti.civil.CaptureStream;
import com.lti.civil.CaptureSystem;
import com.lti.civil.Image;
import com.lti.civil.impl.jni.NativeCaptureSystemFactory;
import com.lti.civil.impl.qtjava.QTCaptureStream;
import com.lti.civil.impl.qtjava.QTCaptureSystem;
import com.lti.civil.impl.qtjava.QTCaptureSystemFactory;
import com.lti.civil.webcam.StoreMostRecent_CaptureObserver;

import drone.jmf.data.BufferedImageToken;
import drone.jmf.data.JMFImageToken;

import ptolemy.actor.lib.Source;
import ptolemy.data.ImageToken;
import ptolemy.data.IntToken;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.StringAttribute;

import javax.media.Buffer;
////VideoCamera
import javax.media.format.VideoFormat;

import net.sf.fmj.media.protocol.civil.DataSource;

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
public class CivilCameraActor extends Source implements CaptureObserver {

	/** Construct an actor with the given container and name.
	 *  @param container The container.
	 *  @param name The name of this actor.
	 *  @exception IllegalActionException If the actor cannot be contained
	 *   by the proposed container.
	 *  @exception NameDuplicationException If the container already has an
	 *   actor with this name.
	 */
	public CivilCameraActor(CompositeEntity container, String name)
	throws IllegalActionException, NameDuplicationException {
		super(container, name);

		deviceNumber = new Parameter(this, "deviceNumber");
		deviceNumber.setExpression("0");

		// FIXME: output should perhaps be named "video"?
		// In case there is audio track.
		// Don't derive from source in this case.
		output.setTypeEquals(BaseType.OBJECT);
	}

	///////////////////////////////////////////////////////////////////
	////                     ports and parameters                  ////

	/** This parameter lets the user select the device to use.
	 *  Typically this parameter is of no concern and should be left
	 *  at 0.  However, if a computer has more than one usable
	 *  cameras, this parameter can be used to choose amongst them.
	 */
	public Parameter deviceNumber;

	///////////////////////////////////////////////////////////////////
	////                         public methods                    ////

//	/** The controller listener.  This method controls the
//	*  initializing of the player.  It also senses when the
//	*  file is done playing, in which case it closes the
//	*  player.
//	*  @param event The controller event.
//	*/
//	public void controllerUpdate(ControllerEvent event) {
//	if (event instanceof ConfigureCompleteEvent
//	|| event instanceof RealizeCompleteEvent
//	|| event instanceof PrefetchCompleteEvent
//	|| event instanceof StartEvent
//	|| event instanceof StopEvent) {
//	synchronized (_waitSync) {
//	_stateTransitionEvent = event;
//	_stateTransitionOK = true;
//	_waitSync.notifyAll();
//	}
//	} else if (event instanceof ControllerClosedEvent) {
//	_stateTransitionEvent = event;
//	_player.close();
//	_playerOpen = false;
//	}
//	// TODO: verify if there are other events to catch
//	}


	/** Capture a frame and send a java.awt.Image object
	 *  to the output port.
	 *  @exception IllegalActionException If there's no director.
	 */
	public void fire() throws IllegalActionException {
		super.fire();
		if (_bufferNew != null) {
			output.send(0, new JMFImageToken(_bufferNew));
		}
	}

	/** Open the file at the URL, and set the width of the output.
	 *  @exception IllegalActionException If there are no video capture
	 *   devices.
	 * @throws CaptureException 
	 */
	public void initialize() throws IllegalActionException {
		super.initialize();

		try {
			_system = (new QTCaptureSystemFactory()).createCaptureSystem();
			List devices = _system.getCaptureDeviceInfoList();
			Iterator it = devices.iterator();
			System.out.println("Available devices =======");
			while (it.hasNext())
				System.out.println(((CaptureDeviceInfo)it.next()).getDeviceID());
			int deviceIndex = ((IntToken)deviceNumber.getToken()).intValue();
			_stream = _system.openCaptureDeviceStream(
					((CaptureDeviceInfo)devices.get(deviceIndex)).getDeviceID());
			_stream.setObserver(this);

			_stream.start();
			System.out.println("Called start on stream");

			synchronized (_waitSync) {
				_playerOpen = true;
			}

			System.out.println("Player started");
		} catch (CaptureException e) {
			throw new IllegalActionException(this, e, "Problem during capture initialization.");
		}
	}

	/** Close the media processor.
	 */
	public void wrapup() throws IllegalActionException {
		if (_stream != null) {
			try {
				_stream.stop();
				_stream.dispose();
				_system.dispose();
			} catch (CaptureException e) {
				throw new IllegalActionException(this, e, "Problem during wrapup.");
			}
		}
	}

//	///////////////////////////////////////////////////////////////////
//	////                         protected methods                 ////

//	/** Block until the processor has transitioned to the given state.
//	*  Return false if the transition failed.
//	*  @param state The state for which we are waiting.
//	*  @return True if the state transition is ok, otherwise return false.
//	*/
//	protected boolean _waitForState(int state) {
//	synchronized (_waitSync) {
//	try {
//	while ((_player.getState() != state) && _stateTransitionOK) {
//	_waitSync.wait();
//	}
//	} catch (Exception e) {
//	}
//	}

//	return _stateTransitionOK;
//	}

//	protected void createDataSource() {

//	}

	///////////////////////////////////////////////////////////////////
	////                         private variables                 ////

	/** The java.awt.Image that we are producing/ */
	private Buffer _bufferNew;

	// Boolean that keeps track of whether the player is open or not.
	private boolean _playerOpen = false;

	private Object _waitSync = new Object();

	private CaptureSystem _system;
	private CaptureStream _stream;
	private StoreMostRecent_CaptureObserver _observer;

	public void onError(CaptureStream stream, CaptureException e) {
		throw new Error("Error caught with stream: " + e.getMessage());
	}

	public void onNewImage(CaptureStream stream, Image image) {
		VideoFormat format = DataSource.convertCivilFormat(image.getFormat(), image.getWidth(), image.getHeight());
		synchronized (_waitSync) {
			if (_playerOpen) {
				if (_bufferNew == null)
					_bufferNew = new Buffer();
				_bufferNew.setData(image.getBytes());
				_bufferNew.setOffset(0);
				_bufferNew.setTimeStamp(System.currentTimeMillis() * 1000000);
				_bufferNew.setLength(image.getBytes().length);
				_bufferNew.setFormat(format);
			}
		}
	}

}
