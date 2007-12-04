/* A set of utility functions wrt operating systems.
 * 
 * Copyright (C) 2007 Jean-Sebastien Senecal (js@drone.ws)
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
package drone.util;

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
	
	/**
	 * Returns true if operating system is Linux.
	 * @return true if operating system is Linux
	 */
	public static final boolean isLinux()
	{
		return System.getProperty("os.name").equals("Linux");
	}
	
	/**
	 * Returns true if operating system is OS X.
	 * @return true if operating system is OS X
	 */
	public static final boolean isMacOSX()
	{
		return System.getProperty("os.name").equals("Mac OS X");
	}
	
	/**
	 * Returns true if operating system is Windows.
	 * @return true if operating system is Windows
	 */
	public static final boolean isWindows()
	{
		return System.getProperty("os.name").startsWith("Windows");
	}

	/**
	 * Returns true if operating system is Solaris.
	 * @return true if operating system is Solaris
	 */
	public static final boolean isSolaris()
	{
		return System.getProperty("os.name").equals("SunOS");
	}

}
