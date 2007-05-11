package drone.artkp.test;

import drone.artkp.ARTKPException;
import drone.artkp.kernel.ARToolKitPlus;
import drone.artkp.kernel.TrackerMultiMarker;
import drone.artkp.kernel.TrackerMultiMarkerImpl;
import junit.framework.TestCase;

public class TrackerMultiMarkerImplTest extends TestCase {

	TrackerMultiMarker tracker;
	final int WIDTH = 320;
	final int HEIGHT = 240;
	final String CAMERA_FILENAME = "/home/tats/eclipse/drone/plugins/drone.artkp/src/drone/artkp/test/LogitechPro4000.dat";
	final String MARKER_FILENAME = "/home/tats/eclipse/drone/plugins/drone.artkp/src/drone/artkp/test/markerboard_480-499.cfg";
	
	public void setUp() throws ARTKPException {
		tracker = new TrackerMultiMarkerImpl();
//		tracker.init(CAMERA_FILENAME, MARKER_FILENAME, 1.0f, 1000.0f);
//		tracker = new TrackerMultiMarkerImpl(WIDTH, HEIGHT);
//		tracker.init(CAMERA_FILENAME, MARKER_FILENAME, 1.0f, 1000.0f);
	}
	
	public void tearDown() {
		tracker.cleanup();
	}
	
	public void testTrackerMultiMarkerImpl() throws ARTKPException {
		new TrackerMultiMarkerImpl();
	}

	public void testTrackerMultiMarkerImplIntInt() throws ARTKPException {
		try {
			new TrackerMultiMarkerImpl(-100, HEIGHT);
			new TrackerMultiMarkerImpl(WIDTH, -100);
			fail("Expected throwing an exception when providing negative height or width value.");
		} catch (Exception e) { }
		new TrackerMultiMarkerImpl(WIDTH, HEIGHT);
	}

	public void testInit() throws ARTKPException {
		assertTrue(tracker.init(CAMERA_FILENAME, MARKER_FILENAME, 1.0f, 1000.0f));
		assertFalse(tracker.init("wrongfile", MARKER_FILENAME, 1.0f, 1000.0f));
		assertFalse(tracker.init(CAMERA_FILENAME, "wrongfile", 1.0f, 1000.0f));
	}
	
	public void testCalc() throws ARTKPException {
		try {
			tracker.calc(new byte[WIDTH*HEIGHT]);
			fail("Exception expected when tryin to call function without yet a camera.");
		} catch (ARTKPException e) {}
		tracker.init(CAMERA_FILENAME, MARKER_FILENAME, 1.0f, 1000.0f);
		tracker.calc(new byte[WIDTH*HEIGHT]);
		try {
			tracker.calc(new byte[0]);
			tracker.calc(new byte[2]);
			tracker.calc(new byte[WIDTH*HEIGHT+2]);
			fail("Exception expected when trying to call function on a wrong-sized array.");
		} catch (Exception e) {}
		try {
			tracker.calc(null);
			fail("Exception expected when trying to call function on null argument.");
		} catch (Exception e) {}
	}

	public void testGetNumDetectedMarkers() throws ARTKPException {
		assertTrue(tracker.getNumDetectedMarkers() >= 0);
	}

	public void testSetUseDetectLite() throws ARTKPException {
		tracker.setUseDetectLite(true);
		tracker.setUseDetectLite(false);
	}

	public void testGetDetectedMarkers() {
		try {
			tracker.getDetectedMarker(-1);
			tracker.getDetectedMarkers(null);
			int[] wrong = new int[] {-1};
			tracker.getDetectedMarkers(wrong);
			fail("Exception expected when trying to access wrongly detected markers.");
		} catch (Exception e) {}
	}


	public void testCalcOpenGLMatrixFromDetectedMarker() {
		fail("Not yet implemented");
	}

	public void testSetPixelFormat() throws ARTKPException {
		assertTrue(tracker.setPixelFormat(ARToolKitPlus.PIXEL_FORMAT_ABGR));
		assertTrue(tracker.setPixelFormat(ARToolKitPlus.PIXEL_FORMAT_BGR));
		assertTrue(tracker.setPixelFormat(ARToolKitPlus.PIXEL_FORMAT_BGRA));
		assertTrue(tracker.setPixelFormat(ARToolKitPlus.PIXEL_FORMAT_LUM));
		assertTrue(tracker.setPixelFormat(ARToolKitPlus.PIXEL_FORMAT_RGB));
		assertTrue(tracker.setPixelFormat(ARToolKitPlus.PIXEL_FORMAT_RGB565));
		assertTrue(tracker.setPixelFormat(ARToolKitPlus.PIXEL_FORMAT_RGBA));
		assertFalse(tracker.setPixelFormat(-1000));
	}

	public void testLoadCameraFile() throws ARTKPException {
		assertTrue(tracker.loadCameraFile(CAMERA_FILENAME, 1.0f, 1000.0f));
		assertFalse(tracker.loadCameraFile("wrongfile", 1.0f, 1000.0f));
	}

	public void testSetLoadUndistLUT() throws ARTKPException {
		tracker.setLoadUndistLUT(true);
		tracker.setLoadUndistLUT(false);
	}

	public void testActivateBinaryMarker() throws ARTKPException {
		tracker.activateBinaryMarker(0);
		tracker.activateBinaryMarker(1);
		tracker.activateBinaryMarker(255);
		tracker.activateBinaryMarker(-1);
	}

	public void testSetMarkerMode() throws ARTKPException {
		tracker.setMarkerMode(ARToolKitPlus.MARKER_ID_BCH);
		tracker.setMarkerMode(ARToolKitPlus.MARKER_ID_SIMPLE);
		tracker.setMarkerMode(ARToolKitPlus.MARKER_TEMPLATE);
	}

	public void testChangeCameraSize() throws ARTKPException {
		try {
			tracker.changeCameraSize(WIDTH*2, HEIGHT*2);
			fail("Expected throwing an exception when trying to change camera size without first calling init().");
		} catch (ARTKPException e) {}

		tracker.init(CAMERA_FILENAME, MARKER_FILENAME, 1.0f, 1000.0f);

		tracker.changeCameraSize(WIDTH*2, HEIGHT*2);
		try {
			tracker.changeCameraSize(-100, HEIGHT);
			tracker.changeCameraSize(WIDTH, -100);
			fail("Expected throwing an exception when providing negative height or width value.");
		} catch (Exception e) { }
	}

	public void testSetUndistortionMode() throws ARTKPException {
		tracker.setUndistortionMode(ARToolKitPlus.UNDIST_LUT);
		tracker.setUndistortionMode(ARToolKitPlus.UNDIST_NONE);
		tracker.setUndistortionMode(ARToolKitPlus.UNDIST_STD);
	}

	public void testSetPoseEstimator() throws ARTKPException {
		tracker.setPoseEstimator(ARToolKitPlus.POSE_ESTIMATOR_ORIGINAL);
		tracker.setPoseEstimator(ARToolKitPlus.POSE_ESTIMATOR_ORIGINAL_CONT);
		tracker.setPoseEstimator(ARToolKitPlus.POSE_ESTIMATOR_RPP);
	}

	public void testSetBorderWidth() throws ARTKPException {
		tracker.setBorderWidth(0.25f);
		tracker.setBorderWidth(0.0f);
		tracker.setBorderWidth(1.0f);
		try {
			tracker.setBorderWidth(-1.0f);
			tracker.setBorderWidth(2.0f);
			fail("Exception expected when trying to set wrong border width.");
		} catch (Exception e) {}
	}

	public void testSetThreshold() throws ARTKPException {
		tracker.setThreshold(0);
		tracker.setThreshold(1);
		tracker.setThreshold(255);
		tracker.setThreshold(-1);
	}

	public void testGetThreshold() throws ARTKPException {
		tracker.setThreshold(100);
		assertEquals(tracker.getThreshold(),100);
	}

	public void testActivateAutoThreshold() throws ARTKPException {
		tracker.activateAutoThreshold(true);
		tracker.activateAutoThreshold(false);
	}

	public void testIsAutoThresholdActivated() throws ARTKPException {
		tracker.activateAutoThreshold(true);
		assertTrue(tracker.isAutoThresholdActivated());
		tracker.activateAutoThreshold(false);
		assertFalse(tracker.isAutoThresholdActivated());
	}

	public void testSetNumAutoThresholdRetries() throws ARTKPException {
		tracker.setNumAutoThresholdRetries(0);
		tracker.setNumAutoThresholdRetries(1);
		tracker.setNumAutoThresholdRetries(2);
		tracker.setNumAutoThresholdRetries(-1);
	}

	public void testSetImageProcessingMode() {
		tracker.setImageProcessingMode(ARToolKitPlus.IMAGE_FULL_RES);
		tracker.setImageProcessingMode(ARToolKitPlus.IMAGE_HALF_RES);
	}

	public void testGetModelViewMatrix() throws ARTKPException {
		assertNotNull(tracker.getModelViewMatrix());
	}

	public void testGetProjectionMatrix() throws ARTKPException {
		assertNotNull(tracker.getProjectionMatrix());
	}

	public void testGetDescription() throws ARTKPException {
		assertNotNull(tracker.getDescription());
	}

	public void testGetPixelFormat() throws ARTKPException {
		tracker.setPixelFormat(ARToolKitPlus.PIXEL_FORMAT_ABGR);
		assertEquals(tracker.getPixelFormat(), ARToolKitPlus.PIXEL_FORMAT_ABGR);
	}

	public void testGetBitsPerPixel() {
		tracker.getBitsPerPixel();
	}

	public void testGetNumLoadablePatterns() {
		tracker.getNumLoadablePatterns();
	}

	public void testCalcOpenGLMatrixFromMarker() {
		fail("Not yet implemented");
	}

}
