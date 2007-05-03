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

import junit.framework.TestCase;
import drone.frei0r.jni.Frei0r;
import drone.frei0r.Frei0rException;
import ptolemy.kernel.util.NameDuplicationException;

public class Frei0rTest extends TestCase {

	public String TEST_LIBRARY_PATH = "contrib/Frei0r/plugins/nois0r.dylib";
	public String TEST_WRONG_LIBRARY = "wrongLibraryName";
	
	public void setUp() throws Exception {
	}

	public void testLoadLibraryWrong() throws NameDuplicationException, Frei0rException
	{
		new Frei0r(TEST_WRONG_LIBRARY);
	}
	
	public void testLoadLibrary() throws NameDuplicationException, Frei0rException
	{
		new Frei0r(TEST_LIBRARY_PATH);
	}

	public void testInit() throws Frei0rException {
		Frei0r frei0r = new Frei0r(TEST_LIBRARY_PATH);
		frei0r.init();
	}
	
	public void testDeinit() throws Frei0rException {
		Frei0r frei0r = new Frei0r(TEST_LIBRARY_PATH);
		frei0r.init();
		frei0r.deinit();
	}
	
	public void testConstruct() throws Frei0rException {
		Frei0r frei0r = new Frei0r(TEST_LIBRARY_PATH);
		frei0r.construct(100, 100);
	}

	public void testDestruct() throws Frei0rException {
		Frei0r frei0r = new Frei0r(TEST_LIBRARY_PATH);
		frei0r.destruct(frei0r.construct(100, 100));
	}
	
	public void testCreateInstance() throws Frei0rException {
		Frei0r frei0r = new Frei0r(TEST_LIBRARY_PATH);
		frei0r.createInstance(100, 100);
	}

	public void testUpdateInstance() throws Frei0rException {
		Frei0r frei0r = new Frei0r(TEST_LIBRARY_PATH);
		Frei0r.Instance instance = frei0r.createInstance(100, 100);
		int[] inframe = new int[100*100];
		int[] outframe = new int[100*100];
		instance.update(0, inframe, outframe);
	}
	
	public void testPluginInfo() throws Frei0rException {
		Frei0r frei0r = new Frei0r(TEST_LIBRARY_PATH);
		assertEquals("Nois0r", frei0r.getName());
		assertEquals("Generates white noise images", frei0r.getExplanation());
		assertEquals("Martin Bayer", frei0r.getAuthor());
	}
	
	public void tearDown() throws Exception {
	}

}
