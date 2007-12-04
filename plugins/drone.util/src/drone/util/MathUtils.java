package drone.util;

public class MathUtils {

	public static double clamp(double value, double min, double max) {
		return Math.min(max, Math.max(min, value));
	}

	public static float clamp(float value, float min, float max) {
		return Math.min(max, Math.max(min, value));
	}

	public static int clamp(int value, int min, int max) {
		return Math.min(max, Math.max(min, value));
	}

	public static long clamp(long value, long min, long max) {
		return Math.min(max, Math.max(min, value));
	}
}
