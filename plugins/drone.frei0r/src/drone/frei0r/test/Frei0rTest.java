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

import java.io.FileNotFoundException;
import java.net.URL;

import junit.framework.TestCase;
import drone.frei0r.jni.Frei0r;
import drone.frei0r.Frei0rException;
import ptolemy.kernel.util.NameDuplicationException;

public class Frei0rTest extends TestCase {

	Frei0r frei0r;
	
	public final String FREI0R_DIRECTORY = "/usr/local/lib/frei0r-1";
	public final String TEST_LIBRARY = "brightness.so";
	public final String TEST_WRONG_LIBRARY = "wrongLibraryName.so";
	
	public String TEST_LIBRARY_NAME = "Brightness";
	public String TEST_LIBRARY_AUTHOR = "Jean-Sebastien Senecal";
	public int TEST_LIBRARY_PLUGIN_TYPE = Frei0r.F0R_PLUGIN_TYPE_FILTER;
	public int TEST_LIBRARY_COLOR_MODEL = Frei0r.F0R_COLOR_MODEL_RGBA8888;
	public int TEST_LIBRARY_FREI0R_VERSION = Frei0r.FREI0R_MAJOR_VERSION;
	public int TEST_LIBRARY_MAJOR_VERSION = 0; 
	public int TEST_LIBRARY_MINOR_VERSION = 2; 
	public int TEST_LIBRARY_NUM_PARAMS =  1; 
	public String TEST_LIBRARY_EXPLANATION = "Adjusts the brightness of a source image";
	
	public String getAbsolutePath(String libraryName) throws FileNotFoundException {
		ClassLoader cl = TestCase.class.getClassLoader();
		URL url = cl.getResource(libraryName);
		if (url == null)
			throw new FileNotFoundException("File not found: " + libraryName);
		return url.getPath();
	}
	
	public void setUp() throws Exception {
		frei0r = new Frei0r(FREI0R_DIRECTORY + "/" + TEST_LIBRARY);
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
	
	public void testLoadLibrary() throws NameDuplicationException, Frei0rException, FileNotFoundException
	{
		frei0r = new Frei0r(FREI0R_DIRECTORY + "/" + TEST_LIBRARY);
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
		frei0r.createInstance(100, 100);
	}

	public void testUpdateInstance() throws Frei0rException {
		Frei0r.Instance instance = frei0r.createInstance(100, 100);
		int[] inframe = new int[100*100];
		int[] outframe = new int[100*100];
		instance.update(0, inframe, outframe);
	}
	
	public void testUpdateCheckImage() throws Frei0rException {
		frei0r = new Frei0r(FREI0R_DIRECTORY + "/invert0r.so");
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
	
	public void testParamBounds() throws Frei0rException {
		frei0r = new Frei0r(FREI0R_DIRECTORY + "/squareblur.so");
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

}
