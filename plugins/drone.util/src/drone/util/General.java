package drone.util;

public class General {
	
	/**
	 * Useful method to get the extension of the native library, depending on current OS.
	 * @return a library extension
	 * @throws Exception if OS is unrecognized
	 */
	public static String getNativeLibraryExtension() throws Exception {
		String os = System.getProperty("os.name");
		if (os.equals("Mac OS X"))
			return "dylib";
		else if (os.equals("Linux"))
			return "so";
		else if (os.equals("Windows"))
			return "dll";
		else
			throw new Exception("Unrecognized OS: " + os);
	}

}
