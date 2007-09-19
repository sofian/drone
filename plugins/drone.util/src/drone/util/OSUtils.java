package drone.util;


/**
 * 
 * @author Ken larson
 *
 */
public final class OSUtils 
{
	private OSUtils()
	{	super();
	}
	
	/**
	 * Useful method to get the extension of the native library, depending on current OS.
	 * @return a library extension
	 * @throws Exception if OS is unrecognized
	 */
	public static String getNativeLibraryExtension() throws Exception {
		String os = System.getProperty("os.name");
		if (isMacOSX())
			return "dylib";
		else if (isLinux())
			return "so";
		else if (isWindows())
			return "dll";
		else
			throw new Exception("Unrecognized OS: " + os);
	}
	
	/**
	 * Useful method to get the extension of the native library, depending on current OS.
	 * @return a library extension
	 * @throws Exception if OS is unrecognized
	 */
	public static String getJNILibraryExtension() throws Exception {
		String os = System.getProperty("os.name");
		if (isMacOSX())
			return "jnilib";
		else if (isLinux())
			return "so";
		else if (isWindows())
			return "dll";
		else
			throw new Exception("Unrecognized OS: " + os);
	}
	
	public static final boolean isLinux()
	{
		return System.getProperty("os.name").equals("Linux");
	}
	
	public static final boolean isMacOSX()
	{
		return System.getProperty("os.name").equals("Mac OS X");
	}
	
	public static final boolean isWindows()
	{
		return System.getProperty("os.name").startsWith("Windows");
	}

	public static final boolean isSolaris()
	{
		return System.getProperty("os.name").equals("SunOS");
	}

}
