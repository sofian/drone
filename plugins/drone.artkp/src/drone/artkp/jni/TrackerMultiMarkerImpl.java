package ptolemy.actor.lib.artkp.kernel;

public class TrackerMultiMarkerImpl extends TrackerMultiMarker {

 	public TrackerMultiMarkerImpl() {
		super();
	}

	public TrackerMultiMarkerImpl(int width, int height) {
		super();
		changeCameraSize(width, height);
	}
	
	protected native long _createTrackerHandle();

	static {
		System.loadLibrary("TrackerMultiMarkerImpl");
	}
}
