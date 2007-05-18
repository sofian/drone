/* The abstract Tracker superclass.
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
package drone.artkp.kernel;

import drone.artkp.ARTKPException;

public abstract class Tracker {

	protected Tracker(long trackerHandle) throws ARTKPException {
		if (trackerHandle == 0)
			throw new ARTKPException("Trying to initialize Tracker with an uninitialized ");
		_trackerHandle = trackerHandle;
		_initTracker();
	}
	
	public void finalize() throws ARTKPException {
		if (_trackerHandle != 0) {
			cleanup();
			_destroyTrackerHandle();
		}
	}
	
	protected native void _initTracker() throws ARTKPException;

	// This is a pointer to the C tracker object. It should be allocated by
	// the subclasses constructors.
	private long _trackerHandle = 0;
	
	private native void _destroyTrackerHandle() throws ARTKPException;
	
	/// does final clean up (memory deallocation)
	public native void cleanup();

	/// Sets the pixel format of the camera image
	/**
	 *  Default format is RGB888 (ARToolkitPlus.PIXEL_FORMAT_RGB)
	 */
	public native boolean setPixelFormat(int nFormat) throws ARTKPException;

	/// Loads a camera calibration file and stores data internally
	/**
	*  To prevent memory leaks, this method internally deletes an existing camera.
	*  If you want to use more than one camera, retrieve the existing camera using getCamera()
	*  and call setCamera(NULL); before loading another camera file.
	*  On destruction, ARToolKitPlus will only destroy the currently set camera. All other
	*  cameras have to be destroyed manually.
	*/
	public native boolean loadCameraFile(String nCamParamFile, float nNearClip, float nFarClip) throws ARTKPException;

	/// Set to true to try loading camera undistortion table from a cache file
	/**
	 *  On slow platforms (e.g. Smartphone) creation of the undistortion lookup-table
	 *  can take quite a while. Consequently caching will speedup the start phase.
	 *  If set to true and no cache file could be found a new one will be created.
	 *  The cache file will get the same name as the camera file with the added extension '.LUT'
	 */
	public native void setLoadUndistLUT(boolean nSet) throws ARTKPException;


//	/// sets an instance which implements the ARToolKit::Logger interface
	// TODO: implement this method
//	public native void setLogger(ARToolKitPlus::Logger* nLogger) ;
//
//
//	/// marker detection using tracking history
	// TODO: implement this method
//	public native int arDetectMarker(ARUint8 *dataPtr, int thresh, ARMarkerInfo **marker_info, int *marker_num) ;
//
//
//	/// marker detection without using tracking history
	// TODO: implement this method
	//	public native int arDetectMarkerLite(ARUint8 *dataPtr, int thresh, ARMarkerInfo **marker_info, int *marker_num) ;


	/// calculates the transformation matrix between camera and the given multi-marker config
	// TODO: implement this method
//	public native ARFloat arMultiGetTransMat(ARMarkerInfo *marker_info, int marker_num, ARMultiMarkerInfoT *config) ;
//	/// calculates the transformation matrix between camera and the given marker
//	// TODO: implement this method
//	public native ARFloat arGetTransMat(ARMarkerInfo *marker_info, ARFloat center[2], ARFloat width, ARFloat conv[3][4]) ;
//
//	// TODO: implement this method
//	public native ARFloat arGetTransMatCont(ARMarkerInfo *marker_info, ARFloat prev_conv[3][4], ARFloat center[2], ARFloat width, ARFloat conv[3][4]) ;
//
//	// RPP integration -- [t.pintaric]
//	// TODO: implement this method
//	public native ARFloat rppMultiGetTransMat(ARMarkerInfo *marker_info, int marker_num, ARMultiMarkerInfoT *config) ;
//	// TODO: implement this method
//	public native ARFloat rppGetTransMat(ARMarkerInfo *marker_info, ARFloat center[2], ARFloat width, ARFloat conv[3][4]) ;
//
//
//	/// loads a pattern from a file
//	// TODO: implement this method
//	public native int arLoadPatt(char *filename) ;
//
//
//	/// frees a pattern from memory
//	// TODO: implement this method
//	public native int arFreePatt(int patno) ;

	/// frees a multimarker config from memory
	// TODO: implement this method
	//public native int arMultiFreeConfig( ARMultiMarkerInfoT *config ) ;

	/// reads a standard artoolkit multimarker config file
	// TODO: implement this method
	//public native ARMultiMarkerInfoT *arMultiReadConfigFile(const char *filename) ;

	/// activates binary markers
	/**
	 *  markers are converted to pure black/white during loading
	 */
	public native void activateBinaryMarker(int nThreshold) throws ARTKPException;

	/// activate the usage of id-based markers rather than template based markers
	/**
	 *  Template markers are the classic marker type used in ARToolKit.
	 *  Id-based markers directly encode the marker id in the image.
	 *  Simple markers use 3-times redundancy to increase robustness, while
	 *  BCH markers use an advanced CRC algorithm to detect and repair marker damages.
	 *  See arBitFieldPattern.h for more information.
	 *  In order to use id-based markers, the marker size has to be 6x6, 12x12 or 18x18.
	 */
	public native void setMarkerMode(int nMarkerMode) throws ARTKPException;


	/// activates the complensation of brightness falloff in the corners of the camera image
	/**
	 *  some cameras have a falloff in brightness at the border of the image, which creates
	 *  problems with thresholding the image. use this function to set a (linear) adapted
	 *  threshold value. the threshold value will stay exactly the same at the center but
	 *  will deviate near to the border. all values specify a difference, not absolute values!
	 *  nCorners define the falloff a all four corners. nLeftRight defines the falloff
	 *  at the half y-position at the left and right side of the image. nTopBottom defines the falloff
	 *  at the half x-position at the top and bottom side of the image.
	 *  all values between these 9 points (center, 4 corners, left, right, top, bottom) will
	 *  be interpolated.
	 */
	// TODO: implement this method
//	public native void activateVignettingCompensation(boolean nEnable, int nCorners=0, int nLeftRight=0, int nTopBottom=0) ;

	
	/// changes the resolution of the camera after the camerafile was already loaded
	public native void changeCameraSize(int nWidth, int nHeight) throws ARTKPException;


	/// Changes the undistortion mode
	/**
	 * Default value is UNDIST_STD which means that
	 * artoolkit's standard undistortion method is used.
	 */
	public native void setUndistortionMode(int nMode) throws ARTKPException;

	/// Changes the Pose Estimation Algorithm
	/**
	* POSE_ESTIMATOR_ORIGINAL (default): arGetTransMat()
	* POSE_ESTIMATOR_RPP: "Robust Pose Estimation from a Planar Target"
	*/
	public native boolean setPoseEstimator(int nMethod) throws ARTKPException;

	/// Sets a new relative border width. ARToolKit's default value is 0.25
	/**
	 * Take caution that the markers need of course really have thiner borders.
	 * Values other than 0.25 have not been tested for regular pattern-based matching,
	 * but only for id-encoded markers. It might be that the pattern creation process
	 * needs to be updated too.
	 */
	public native void setBorderWidth(float nFraction) throws ARTKPException;


	/// Sets the threshold value that is used for black/white conversion
	public native void setThreshold(int nValue) throws ARTKPException;


	/// Returns the current threshold value.
	public native int getThreshold() throws ARTKPException;


	/// Enables or disables automatic threshold calculation
	public native void activateAutoThreshold(boolean nEnable) throws ARTKPException;

	
	/// Returns true if automatic threshold calculation is activated
	public native boolean isAutoThresholdActivated() throws ARTKPException;

	/// Sets the number of times the threshold is randomized in case no marker was visible (Default: 2)
	/**
	 *  Autothreshold requires a visible marker to estime the optimal thresholding value. If
	 *  no marker is visible ARToolKitPlus randomizes the thresholding value until a marker is
	 *  found. This function sets the number of times ARToolKitPlus will randomize the threshold
	 *  value and research for a marker per calc() invokation until it gives up.
	 *  A value of 2 means that ARToolKitPlus will analyze the image a second time with an other treshold value
	 *  if it does not find a marker the first time. Each unsuccessful try uses less processing power
	 *  than a single full successful position estimation.
	 */
	public native void setNumAutoThresholdRetries(int nNumRetries) throws ARTKPException;


	/// Sets an image processing mode (half or full resolution)
	/**
	 *  Half resolution is faster but less accurate. When using
	 *  full resolution smaller markers will be detected at a
	 *  higher accuracy (or even detected at all).
	 */
	public native void setImageProcessingMode(int nMode) ;


	/// Returns an opengl-style modelview transformation matrix
	public native float[] getModelViewMatrix() throws ARTKPException;


	/// Returns an opengl-style projection transformation matrix
	public native float[] getProjectionMatrix() throws ARTKPException;


	/// Returns a short description with compiled-in settings
	public native String getDescription() throws ARTKPException;


	/// Returns the compiled-in pixel format
	public native int getPixelFormat() throws ARTKPException;


	/// Returns the number of bits required to store a single pixel
	public native int getBitsPerPixel();

	/// Returns the maximum number of patterns that can be loaded
	/**
	 *  This maximum number of loadable patterns can be set via the
	 *  __MAX_LOAD_PATTERNS template parameter
	 */
	public native int getNumLoadablePatterns();


	/// Returns the current camera
	// TODO: implement this method
//	public native Camera* getCamera() ;
//
//
//	/// Sets a new camera without specifying new near and far clip values
	// TODO: implement this method
//	public native void setCamera(Camera* nCamera) ;


	/// Sets a new camera including specifying new near and far clip values
	// TODO: implement this method
//	public native void setCamera(Camera* nCamera, ARFloat nNearClip, ARFloat nFarClip) ;


	/// Calculates the OpenGL transformation matrix for a specific marker info
	public native float calcOpenGLMatrixFromMarker(long nMarkerInfo, float nPatternCenterX, float nPatternCenterY, float nPatternSize, float[] nOpenGLMatrix) ;


	/// Returns the internal profiler object
///	public native Profiler& getProfiler() ;


	/// Calls the pose estimator set with setPoseEstimator() for single marker tracking
	// TODO: implement this method
	//public native ARFloat executeSingleMarkerPoseEstimator(ARMarkerInfo *marker_info, ARFloat center[2], ARFloat width, ARFloat conv[3][4]) ;

	/// Calls the pose estimator set with setPoseEstimator() for multi marker tracking
	// TODO: implement this method
	//public native ARFloat executeMultiMarkerPoseEstimator(ARMarkerInfo *marker_info, int marker_num, ARMultiMarkerInfoT *config) ;

	static {
		System.loadLibrary("drone_artkp_kernel_Tracker");
	}
}
