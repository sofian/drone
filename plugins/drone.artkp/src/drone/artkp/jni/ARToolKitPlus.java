package ptolemy.actor.lib.artkp.kernel;

public class ARToolKitPlus {
	
	// PIXEL_FORMAT
	public static final int PIXEL_FORMAT_ABGR = 1;
	public static final int PIXEL_FORMAT_BGRA = 2;
	public static final int PIXEL_FORMAT_BGR = 3;
	public static final int PIXEL_FORMAT_RGBA = 4;
	public static final int PIXEL_FORMAT_RGB = 5;
	public static final int PIXEL_FORMAT_RGB565 = 6;
	public static final int PIXEL_FORMAT_LUM = 7;
	
	// UNDIST_MODE
	public static final int UNDIST_NONE = 0;
	public static final int UNDIST_STD = 1;
	public static final int UNDIST_LUT = 2;


	// IMAGE_PROC_MODE
	public static final int IMAGE_HALF_RES = 0;
	public static final int IMAGE_FULL_RES = 1;

	// MARKER_MODE
	public static final int MARKER_TEMPLATE = 0;
	public static final int MARKER_ID_SIMPLE = 1;
	public static final int MARKER_ID_BCH = 2;

	// POSE_ESTIMATOR
	public static final int POSE_ESTIMATOR_ORIGINAL = 0;			// original "normal" pose estimator
	public static final int POSE_ESTIMATOR_ORIGINAL_CONT = 1;		// original "cont" pose estimator
	public static final int POSE_ESTIMATOR_RPP = 2;					// new "Robust Planar Pose" estimator

}
