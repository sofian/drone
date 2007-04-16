package ptolemy.actor.lib.artkp.kernel;

public abstract class TrackerMultiMarker extends Tracker {

	/// initializes ARToolKit
	/// initializes TrackerSingleMarker
	/**
	 *  nCamParamFile is the name of the camera parameter file
	 *  nNearClip & nFarClip are near and far clipping values for the OpenGL projection matrix
	 *  nLogger is an instance which implements the ARToolKit::Logger interface
	 */
	public native boolean init(String nCamParamFile, String nMultiFile, float nNearClip, float nFarClip/*, ARToolKitPlus::Logger* nLogger=NULL*/);

	/// calculates the transformation matrix
	/**
	 *	pass the image as RGBX (32-bits) in 320x240 pixels.
	 */
	public native int calc(byte[] nImage);

	/// Returns the number of detected markers used for multi-marker tracking
	public native int getNumDetectedMarkers();

	/// Enables usage of arDetectMarkerLite. Otherwise arDetectMarker is used
	/**
	 * In general arDetectMarker is more powerful since it keeps history about markers.
	 * In some cases such as very low camera refresh rates it is advantegous to change this.
	 * Using the non-lite version treats each image independent.
	 */
	public native void setUseDetectLite(boolean nEnable);

	/// Returns array of detected marker IDs
	/**
	 * Only access the first getNumDetectedMarkers() markers
	 */
	public native void getDetectedMarkers(int[] nMarkerIDs);

	/// Returns the ARMarkerInfo object for a found marker
	public native long getDetectedMarker(int nWhich);
	
	/// Calculates the OpenGL transformation matrix for the specified detected markers.
	public native float calcOpenGLMatrixFromDetectedMarker(int nWhich, float nPatternCenterX, float nPatternCenterY, float nPatternSize, float[] nOpenGLMatrix);
	
	/// Returns the loaded ARMultiMarkerInfoT object
	/**
	 *  If loading the multi-marker config file failed then this method
	 *  returns NULL.
	 */
//	public native const ARMultiMarkerInfoT* getMultiMarkerConfig() const = 0;


	/// Provides access to ARToolKit' internal version of the transformation matrix
	/**
	*  This method is primarily for compatibility issues with code previously using
	*  ARToolKit rather than ARToolKitPlus. This is the original transformation
	*  matrix ARToolKit calculates rather than the OpenGL style version of this matrix
	*  that can be retrieved via getModelViewMatrix().
	*/
//	public native void getARMatrix(ARFloat nMatrix[3][4]) const = 0;
	

	static {
		System.loadLibrary("TrackerMultiMarker");
	}
}
