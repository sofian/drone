/* An actor that takes an image in input and outputs informations about augmented reality
 * markers found in the image.
 *  
 * Copyright (c) 2006 Jean-Sebastien Senecal (js@drone.ws)
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
package drone.artkp.actor;

import java.awt.Graphics;
import java.awt.GraphicsConfiguration;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.HeadlessException;
import java.awt.Image;
import java.awt.Transparency;
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;
import java.awt.image.DataBuffer;
import java.awt.image.DataBufferByte;
import java.awt.image.DataBufferInt;
import java.awt.image.PixelGrabber;
import java.io.File;
import java.util.TreeMap;
import java.util.Vector;

import javax.swing.ImageIcon;

import ptolemy.actor.lib.Transformer;
import drone.artkp.kernel.*;
import ptolemy.actor.parameters.DoubleRangeParameter;
import ptolemy.data.ArrayToken;
import ptolemy.data.BooleanToken;
import ptolemy.data.DoubleMatrixToken;
import ptolemy.data.DoubleToken;
import ptolemy.data.ImageToken;
import ptolemy.data.IntToken;
import ptolemy.data.ObjectToken;
import ptolemy.data.RecordToken;
import ptolemy.data.ScalarToken;
import ptolemy.data.Token;
import ptolemy.data.expr.FileParameter;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.ArrayType;
import ptolemy.data.type.BaseType;
import ptolemy.data.type.RecordType;
import ptolemy.data.type.Type;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

//////////////////////////////////////////////////////////////////////////
////StringCompare

/**
 * Provide a bridge for Frei0r, minimalistic plugin API for video effects.
 * http://www.piksel.org/frei0r/1.0/spec/index.html
 * 
 * 
 * 
 * @author Mathieu Guindon
 */
public class ARTKPActor extends Transformer {
	
	private float _patternSize;
	
	private int _camWidth = -1;
	private int _camHeight = -1;
	
	Parameter patternSize;
	
	Parameter threshold;

	Parameter nearClipping;
	Parameter farClipping;
	Parameter useDetectLite;
	
	FileParameter camParamFile;
	FileParameter multiFile;
	
	/**
	 * Construct an actor with the given container and name. Construct the two
	 * operand input PortParameters (initialized to "") and the output port
	 * which outputs the result of the various comparison functions executed by
	 * the actor. The function to be executed is decided by the parameter
	 * <i>function</i>, which is also initialized here to the comparison
	 * function equals. The <i>ignoreCase</i> parameter allows to ignore case
	 * when comparing.
	 * 
	 * @param container
	 *            The container.
	 * @param name
	 *            The name of this actor.
	 * @exception IllegalActionException
	 *                If the actor cannot be contained by the proposed
	 *                container.
	 * @exception NameDuplicationException
	 *                If the container already has an actor with this name.
	 */
	public ARTKPActor(CompositeEntity container, String name)
	throws NameDuplicationException, IllegalActionException {
		super(container, name);
		
		// FIXME: This is required to be an ImageToken, but
		// we don't see to have that class.
		input.setTypeEquals(BaseType.OBJECT);
		
		output.setTypeEquals(ARTKPActor.TRACKER_INFO_TYPE);
		
		_tracker = new TrackerMultiMarkerImpl();
		
		patternSize = new Parameter(this, "patternSize");
		patternSize.setExpression("80.0");
		patternSize.setTypeEquals(BaseType.DOUBLE);

		nearClipping = new Parameter(this, "nearClipping");
		nearClipping.setExpression("1.0");
		nearClipping.setTypeEquals(BaseType.DOUBLE);
		
		farClipping = new Parameter(this, "farClipping");
		farClipping.setExpression("1000.0");
		farClipping.setTypeEquals(BaseType.DOUBLE);
		
		threshold = new Parameter(this, "threshold");
		threshold.setExpression("160");
		threshold.setTypeEquals(BaseType.INT);

		camParamFile = new FileParameter(this, "cameraParameterFile");
		camParamFile.setTypeEquals(BaseType.STRING);
		camParamFile.setExpression("$CLASSPATH/ptolemy/actor/lib/artkp/camera_para.dat");
		
		multiFile = new FileParameter(this, "multiMarkerFile");
		multiFile.setTypeEquals(BaseType.STRING);
		multiFile.setExpression("$CLASSPATH/ptolemy/actor/lib/artkp/markerboard_480-499.cfg");
		
		useDetectLite = new Parameter(this, "useDetectLite");
		useDetectLite.setExpression("false");
		
		_attachText("_iconDescription", "<svg>\n"
				+ "<rect x=\"-30\" y=\"-15\" " + "width=\"60\" height=\"30\" "
				+ "style=\"fill:white\"/>\n" + "</svg>\n");
	}
	
	public void initialize() throws IllegalActionException {

//		System.out.println("init with + " + _camParamFile + " " + _multiFile + " " + _nearClipping + " " + _farClipping);
	    // load a camera file. two types of camera files are supported:
	    //  - Std. ARToolKit
	    //  - MATLAB Camera Calibration Toolbox
		File cFile = camParamFile.asFile();
		if (cFile == null)
			throw new IllegalActionException("Given camera parameters file cannot be opened.");

		File mFile = multiFile.asFile();
		if (mFile == null)
			throw new IllegalActionException("Given multi file cannot be opened.");
		
		_tracker.init(cFile.getAbsolutePath(), mFile.getAbsolutePath(),
				     (float)((ScalarToken)nearClipping.getToken()).doubleValue(), 
				     (float)((ScalarToken)farClipping.getToken()).doubleValue());
				     
//		System.out.println("pixel format");
		_tracker.setPixelFormat(ARToolKitPlus.PIXEL_FORMAT_LUM);
		
//		System.out.println("set border width");
		// the marker in the BCH test image has a thiner border...
//	    _tracker.setBorderWidth(0.125f);
	    _tracker.setBorderWidth(0.25f);

	    // set a threshold. we could also activate automatic thresholding
//		System.out.println("set threshold");
	    _tracker.setThreshold(((IntToken)threshold.getToken()).intValue());

	    // let's use lookup-table undistortion for high-speed
	    // note: LUT only works with images up to 1024x1024
//		System.out.println("set undist");
	    _tracker.setUndistortionMode(ARToolKitPlus.UNDIST_LUT);

	    _tracker.setUseDetectLite(((BooleanToken)useDetectLite.getToken()).booleanValue());
	    
	    _tracker.setPoseEstimator(ARToolKitPlus.POSE_ESTIMATOR_ORIGINAL);
//	    _tracker.setPoseEstimator(ARToolKitPlus.POSE_ESTIMATOR_RPP);
	    // RPP is more robust than ARToolKit's standard pose estimator
	    //tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);

	    // switch to simple ID based markers
	    // use the tool in tools/IdPatGen to generate markers
//		System.out.println("set marker mode");
	    _tracker.setMarkerMode(ARToolKitPlus.MARKER_ID_SIMPLE);
//	    _tracker.setMarkerMode(ARToolKitPlus.MARKER_ID_BCH);
	}
	
	public void attributeChanged(Attribute attribute)
	throws IllegalActionException {
		if (attribute == patternSize) {
			_patternSize = (float) ((DoubleToken)patternSize.getToken()).doubleValue();
		}
		
		super.attributeChanged(attribute);
	}
	
	// /////////////////////////////////////////////////////////////////
	// // ports and parameters ////
	
	// /////////////////////////////////////////////////////////////////
	// // public methods ////
	
	/**
	 * Consume exactly one input token from each input port, and compute the
	 * specified string function of the input taking into account the
	 * <i>ignoreCase</i> parameter.
	 * 
	 * @exception IllegalActionException
	 *                If there is no director.
	 */
	public void fire() throws IllegalActionException {
		super.fire();
		if (input.getWidth() > 0 && input.hasToken(0)) {
			Image image = ((ImageToken)input.get(0)).asAWTImage();
			if (image != null) {
				BufferedImage bimage = toGrayBufferedImage(image);
				if (_camWidth != bimage.getWidth() || _camHeight != bimage.getHeight()) {
					_camWidth = bimage.getWidth();
					_camHeight = bimage.getHeight();
					System.out.println("New camera size detected: " + _camWidth + "x" + _camHeight);
					_tracker.changeCameraSize(_camWidth, _camHeight);
				}
				DataBuffer buffer = bimage.getData().getDataBuffer();
				if (buffer.getDataType() != DataBuffer.TYPE_BYTE) {
					throw new IllegalActionException(this,
					"Data has wrong type.");
				}
				byte[] imageBuffer = ((DataBufferByte)buffer).getData();
				if (imageBuffer.length != bimage.getWidth() * bimage.getHeight())
					throw new IllegalActionException(this, 
							"Image buffer has wrong length " + imageBuffer.length + ".");
				_tracker.calc(imageBuffer);
				int nMarkers = _tracker.getNumDetectedMarkers();
				int[] markerIDs = new int[nMarkers];
				float[][] markerMatrix = new float[nMarkers][16];
				float[] projectionMatrix = _tracker.getProjectionMatrix();
//				System.out.println("Num. detected markers: " + nMarkers);
//				System.out.println("Trying to get the ids");
				_tracker.getDetectedMarkers(markerIDs);
				for (int i=0; i<nMarkers; i++) {
//					System.out.print(markerIDs[i] + " ");
					_tracker.calcOpenGLMatrixFromDetectedMarker(i,
							0.0f,0.0f,_patternSize,
							markerMatrix[i]
							);
				}
//				System.out.println();
//				System.out.println("---------");
				
				TreeMap record = new TreeMap();
				record.put("projectionMatrix", new ObjectToken(projectionMatrix));
				IntToken[] tokenMarkerIDs = new IntToken[markerIDs.length];
				for (int i=0; i<tokenMarkerIDs.length; i++)
					tokenMarkerIDs[i] = new IntToken(markerIDs[i]);
				record.put("markerIds", new ArrayToken(BaseType.INT, tokenMarkerIDs));
				ObjectToken[] tokenMarkerMatrix = new ObjectToken[markerMatrix.length];
				for (int i=0; i<tokenMarkerMatrix.length; i++)
					tokenMarkerMatrix[i] = new ObjectToken(markerMatrix[i]);
				record.put("markerMatrix", new ArrayToken(BaseType.OBJECT, tokenMarkerMatrix));
				output.send(0, new RecordToken(record));
			}
		}
		
	}
	
	public static final RecordType TRACKER_INFO_TYPE = new RecordType(
		new String[] { "projectionMatrix", "markerIds", "markerMatrix" },
		new Type[] { BaseType.OBJECT, new ArrayType(BaseType.INT), new ArrayType(BaseType.OBJECT) }
	);
	
	// /////////////////////////////////////////////////////////////////
	// // private variables ////
	private TrackerMultiMarker _tracker = null;
//	private float[] _modelViewMatrix = new float[16];
//	
//	private float[] _projectionMatrix = new float[16];
	
////	This method returns a buffered image with the contents of an image
//	public static BufferedImage toBufferedImage(Image image) {
//	if (image instanceof BufferedImage) {
//	return (BufferedImage)image;
//	}
//	
//	// This code ensures that all the pixels in the image are loaded
//	image = new ImageIcon(image).getImage();
//	
//	// Determine if the image has transparent pixels; for this method's
//	// implementation, see e661 Determining If an Image Has Transparent Pixels
//	boolean hasAlpha = hasAlpha(image);
//	
//	// Create a buffered image with a format that's compatible with the screen
//	BufferedImage bimage = null;
//	GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
//	try {
//	// Determine the type of transparency of the new buffered image
//	int transparency = Transparency.OPAQUE;
//	if (hasAlpha) {
//	transparency = Transparency.BITMASK;
//	}
//	
//	// Create the buffered image
//	GraphicsDevice gs = ge.getDefaultScreenDevice();
//	GraphicsConfiguration gc = gs.getDefaultConfiguration();
//	bimage = gc.createCompatibleImage(
//	image.getWidth(null), image.getHeight(null), transparency);
//	} catch (HeadlessException e) {
//	// The system does not have a screen
//	}
//	
//	if (bimage == null) {
//	// Create a buffered image using the default color model
//	int type = BufferedImage.TYPE_INT_RGB;
//	if (hasAlpha) {
//	type = BufferedImage.TYPE_INT_ARGB;
//	}
//	bimage = new BufferedImage(image.getWidth(null), image.getHeight(null), type);
//	}
//	
//	// Copy image to buffered image
//	Graphics g = bimage.createGraphics();
//	
//	// Paint the image onto the buffered image
//	g.drawImage(image, 0, 0, null);
//	g.dispose();
//	
//	return bimage;
//	}
//	
////	This method returns true if the specified image has transparent pixels
//	public static boolean hasAlpha(Image image) {
//	// If buffered image, the color model is readily available
//	if (image instanceof BufferedImage) {
//	BufferedImage bimage = (BufferedImage)image;
//	return bimage.getColorModel().hasAlpha();
//	}
//	
//	// Use a pixel grabber to retrieve the image's color model;
//	// grabbing a single pixel is usually sufficient
//	PixelGrabber pg = new PixelGrabber(image, 0, 0, 1, 1, false);
//	try {
//	pg.grabPixels();
//	} catch (InterruptedException e) {
//	}
//	
//	// Get the image's color model
//	ColorModel cm = pg.getColorModel();
//	return cm.hasAlpha();
//	}
//	
	public static BufferedImage toGrayBufferedImage(Image image) {
	if (image instanceof BufferedImage && ((BufferedImage)image).getType() == BufferedImage.TYPE_BYTE_GRAY)
	return (BufferedImage)image;
	// This code ensures that all the pixels in the image are loaded
	image = new ImageIcon(image).getImage();
	// Create a buffered image with a format that's compatible with the screen
	BufferedImage bimage = new BufferedImage(image.getWidth(null), image.getHeight(null), BufferedImage.TYPE_BYTE_GRAY);
	
	// Copy image to buffered image
	Graphics g = bimage.createGraphics();
	
	// Paint the image onto the buffered image
	g.drawImage(image, 0, 0, null);
	g.dispose();
	
	return bimage;
	}
}
