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
