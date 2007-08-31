/* 
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
package drone.frei0r.test;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FilenameFilter;
import java.net.URL;

import junit.framework.TestCase;
import ptolemy.kernel.util.NameDuplicationException;
import drone.frei0r.Frei0rException;
import drone.frei0r.jni.Frei0r;

public class Frei0rTest extends TestCase {

	Frei0r frei0r;

	public static final String FREI0R_DIRECTORY = "/usr/local/lib/frei0r-1";
	public static final String TEST_LIBRARY = "brightness";
	public static final String TEST_WRONG_LIBRARY = "wrongLibraryName";

	public static final String TEST_LIBRARY_NAME = "Brightness";
	public static final String TEST_LIBRARY_AUTHOR = "Jean-Sebastien Senecal";
	public static final int TEST_LIBRARY_PLUGIN_TYPE = Frei0r.F0R_PLUGIN_TYPE_FILTER;
	public static final int TEST_LIBRARY_COLOR_MODEL = Frei0r.F0R_COLOR_MODEL_RGBA8888;
	public static final int TEST_LIBRARY_FREI0R_VERSION = Frei0r.FREI0R_MAJOR_VERSION;
	public static final int TEST_LIBRARY_MAJOR_VERSION = 0; 
	public static final int TEST_LIBRARY_MINOR_VERSION = 2; 
	public static final int TEST_LIBRARY_NUM_PARAMS =  1; 
	public static final String TEST_LIBRARY_EXPLANATION = "Adjusts the brightness of a source image";

	public String getAbsolutePath(String libraryName) throws FileNotFoundException {
		ClassLoader cl = TestCase.class.getClassLoader();
		URL url = cl.getResource(libraryName);
		if (url == null)
			throw new FileNotFoundException("File not found: " + libraryName);
		return url.getPath();
	}

	public static String getLibraryExtension() throws Exception {
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
	
	public void setUp() throws Exception {
		frei0r = new Frei0r(FREI0R_DIRECTORY + "/" + TEST_LIBRARY + "." + getLibraryExtension());
		frei0r.init();
	}


	public void tearDown() throws Exception {
		frei0r.deinit();
	}

	/*****************************************
	 * Library loading tests 
	 *****************************************/ 
	public void testLoadLibraryWrong() throws NameDuplicationException, Frei0rException
	{
		try {
			new Frei0r(TEST_WRONG_LIBRARY);
		} catch (Frei0rException e) {
			return;
		}
		fail("This call should have generated a Frei0rException.");
	}

	public void testLoadLibrary() throws Exception
	{
		frei0r = new Frei0r(FREI0R_DIRECTORY + "/" + TEST_LIBRARY + "." + getLibraryExtension());
	}
	
	public void testLoadLibrariesHeavy() throws Exception
	{
		String[] files = _getAllLibraries();
		if (files != null) {
			// Load all of them 5 times
			for (int k=0; k<5; k++)
				for (int i=0; i<files.length; i++) {
					Frei0r testFrei0r = new Frei0r(FREI0R_DIRECTORY + "/" + files[i]);
					testFrei0r.init();
					testFrei0r.createInstance(100, 100);
					testFrei0r.deinit();
				}
		}
	}
	
	/*****************************************
	 * Public methods tests 
	 *****************************************/ 

	public void testGetName() throws Frei0rException {
		assertEquals(frei0r.getName(), TEST_LIBRARY_NAME);
	}

	public void testGetAuthor() throws Frei0rException {
		assertEquals(frei0r.getAuthor(), TEST_LIBRARY_AUTHOR);
	}

	public void testGetPluginType() throws Frei0rException {
		assertEquals(frei0r.getPluginType(), TEST_LIBRARY_PLUGIN_TYPE);
	}

	public void testGetColorModel() throws Frei0rException {
		assertEquals(frei0r.getColorModel(), TEST_LIBRARY_COLOR_MODEL);
	}

	public void testGetFrei0rVersion() throws Frei0rException {
		assertEquals(frei0r.getFrei0rVersion(), TEST_LIBRARY_FREI0R_VERSION);
	}

	public void testGetMajorVersion() throws Frei0rException {
		assertEquals(frei0r.getMajorVersion(), TEST_LIBRARY_MAJOR_VERSION);
	}

	public void testGetMinorVersion() throws Frei0rException {
		assertEquals(frei0r.getMinorVersion(), TEST_LIBRARY_MINOR_VERSION);
	}

	public void testNParams() throws Frei0rException {
		assertEquals(frei0r.nParams(), TEST_LIBRARY_NUM_PARAMS);
	}

	public void testGetExplanation() throws Frei0rException {
		assertEquals(frei0r.getExplanation(), TEST_LIBRARY_EXPLANATION);
	}

	/*****************************************
	 * Protected methods tests 
	 *****************************************/ 

	public void testInit() throws Frei0rException {
		frei0r.init();
	}

	public void testDeinit() throws Frei0rException {
		frei0r.init();
		frei0r.deinit();
	}

	public void testConstruct() throws Frei0rException {
		frei0r.construct(100, 100);
	}

	public void testDestruct() throws Frei0rException {
		frei0r.destruct(frei0r.construct(100, 100));
	}

	public void testCreateInstance() throws Frei0rException {
		frei0r.createInstance(0, 0);
		frei0r.createInstance(100, 100);
		frei0r.createInstance(1000, 1000);
		try {
			frei0r.createInstance(-1, 100);
			fail("Expected Frei0rException.");
		} catch (Frei0rException e) {}
		try {
			frei0r.createInstance(100, -1);
			fail("Expected Frei0rException.");
		} catch (Frei0rException e) {}
	}

	public void testUpdateInstance() throws Frei0rException {
		Frei0r.Instance instance = frei0r.createInstance(100, 100);
		int[] inframe = new int[100*100];
		int[] outframe = new int[100*100];
		instance.update(0, inframe, outframe);
	}

	public void testUpdateCheckImage() throws Exception {
		frei0r = new Frei0r(FREI0R_DIRECTORY + "/invert0r." + getLibraryExtension());
		Frei0r.Instance instance = frei0r.createInstance(100, 100);
		int[] inframe = new int[100*100];
		// Init with dummy data.
		for (int i=0; i<inframe.length; i++)
			inframe[i] = i;
		int[] inverted = new int[100*100];
		int[] reinverted = new int[100*100];
		instance.update(0, inframe, inverted);
		instance.update(0, inverted, reinverted);
		// Check if they are the same
		for (int i=0; i<inframe.length; i++) {
			assertTrue(inframe[i] != inverted[i]); // Inverted should differ
			assertEquals(inframe[i], reinverted[i]); // ... but reinverted should be same
		}
	}

	public void testUpdateCheckImageMixer2() throws Exception {
		frei0r = new Frei0r(FREI0R_DIRECTORY + "/blend." + getLibraryExtension());
		Frei0r.Instance instance = frei0r.createInstance(100, 100);
		int[] inframe1 = new int[100*100];
		int[] inframe2 = new int[100*100];
		// Init with dummy data.
		for (int i=0; i<inframe1.length; i++) {
			inframe1[i] = inframe2[i] = 0xee0000ee;
		}
		int[] blended = new int[100*100];
		instance.update2(0, inframe1, inframe2, null, blended);
		// Check if they are the same
		for (int i=0; i<inframe1.length; i++) {
			int in0 = (inframe1[i] >> 24) & 0xFF;  // get pixel bytes in ARGB order
			int in1 = (inframe1[i] >> 16) & 0xFF;
			int in2 = (inframe1[i] >> 8) & 0xFF;
			int in3 = (inframe1[i] >> 0) & 0xFF;
			int b0 = (blended[i] >> 24) & 0xFF;  // get pixel bytes in ARGB order
			int b1 = (blended[i] >> 16) & 0xFF;
			int b2 = (blended[i] >> 8) & 0xFF;
			int b3 = (blended[i] >> 0) & 0xFF;
			
			assertTrue("Blended image differs at " + i + ": image = " +
							in0 + " " + in1 + " " + in2 + " " + in3 + ", blended =" +
							b0 + " " + b1 + " " + b2 + " " + b3, blended[i] == inframe1[i]);
		}
	}

	public void testParamBounds() throws Exception {
		frei0r = new Frei0r(FREI0R_DIRECTORY + "/squareblur." + getLibraryExtension());
		Frei0r.Instance instance = frei0r.createInstance(100, 100);
		int[] inframe = new int[100*100];
		int[] outframe = new int[100*100];
		// Init with dummy data.
		for (int i=0; i<inframe.length; i++)
			inframe[i] = i;
		instance.setParamValue(new Double(-1.0), 0);
		instance.update(0, inframe, outframe);
		instance.setParamValue(new Double(2.0), 0);
		instance.update(0, inframe, outframe);
	}

	public void testSendWrongInputsMixer2() throws Exception {
		frei0r = new Frei0r(FREI0R_DIRECTORY + "/hue." + getLibraryExtension());
		assertEquals(Frei0r.F0R_PLUGIN_TYPE_MIXER2, frei0r.getPluginType());
		Frei0r.Instance instance = frei0r.createInstance(100, 100);
		BufferedImage in1 = new BufferedImage(100, 100, BufferedImage.TYPE_INT_ARGB);
		BufferedImage in2 = new BufferedImage(100, 100, BufferedImage.TYPE_INT_ARGB);
		BufferedImage out = new BufferedImage(100, 100, BufferedImage.TYPE_INT_ARGB);
		BufferedImage badIn1 = new BufferedImage(1, 100, BufferedImage.TYPE_INT_ARGB);
		BufferedImage badIn2 = new BufferedImage(100, 1, BufferedImage.TYPE_INT_ARGB);
		BufferedImage badOut = new BufferedImage(888, 100, BufferedImage.TYPE_INT_ARGB);
		try {
			instance.update2(0, in1, null, null, out);
			fail("Expected NullPointerException.");
		} catch (NullPointerException e) {}
		try {
			instance.update2(0, null, in2, null, out);
			fail("Expected NullPointerException.");
		} catch (NullPointerException e) {}
		try {
			instance.update2(0, in1, in2, null, null);
			fail("Expected NullPointerException.");
		} catch (NullPointerException e) {}
		try {
			instance.update2(0, badIn1, in2, null, out);
			fail("Expected Exception.");
		} catch (Exception e) {}
		try {
			instance.update2(0, in1, badIn2, null, out);
			fail("Expected Exception.");
		} catch (Exception e) {}
		try {
			instance.update2(0, in1, in2, null, badOut);
			fail("Expected Exception.");
		} catch (Exception e) {}
		int[] inframe1 = new int[100*100];
		int[] inframe2 = new int[100*100];
		int[] outframe = new int[100*100];
		try {
			instance.update2(0, inframe1, null, null, outframe);
			fail("Expected NullPointerException.");
		} catch (NullPointerException e) {}
		try {
			instance.update2(0, null, inframe2, null, outframe);
			fail("Expected NullPointerException.");
		} catch (NullPointerException e) {}
		try {
			instance.update2(0, inframe1, inframe2, null, null);
			fail("Expected NullPointerException.");
		} catch (NullPointerException e) {}
		try {
			instance.update2(0, inframe1, new int[100], null, outframe);
			fail("Expected Exception.");
		} catch (Exception e) {}
		try {
			instance.update2(0, new int[100], inframe2, null, outframe);
			fail("Expected Exception.");
		} catch (Exception e) {}
		try {
			instance.update2(0, inframe1, inframe2, null, new int[1000]);
			fail("Expected Exception.");
		} catch (Exception e) {}
		// this is normal
		instance.update2(0, inframe1, inframe2, null, outframe);
	}
	
	private static String[] _getAllLibraries() {
		File dir = new File(FREI0R_DIRECTORY);
		String[] files = dir.list(new FilenameFilter() {
			public boolean accept(File dir, String name) {
				try {
					return (name.endsWith("." + Frei0rTest.getLibraryExtension()));
				} catch (Exception e) {
					return false;
				}
		    }
		});
		return files;
	}

}
