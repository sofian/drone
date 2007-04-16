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
package drone.frei0r.tests;

import org.junit.*;
import ptolemy.actor.TypedCompositeActor;
import drone.frei0r.actors.*;
import drone.frei0r.*;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

public class Frei0rActorTest {

	private TypedCompositeActor _top;
	@Before
	public void setUp() throws Exception {
		_top = new TypedCompositeActor();
		_top.setName("Frei0r unit tests");
	}

	/**
	 * Nothing specific to test in the constructor, but
	 * the frei0rJNI lib is loaded in a static statement,
	 * so any problems with the native lib will be reported
	 * at the first instanciation.
	 */
	@Test 
	public void testConstructor()
	{
		try
		{
			new Frei0rActor(_top, "frei0rTest");
			
		} catch (Exception e) {
			Assert.fail(e.getMessage());
		}
	}
	
	@Test
	public void testSetFrei0rLibraryNameAttributeWrong() throws NameDuplicationException, IllegalActionException
	{
		Frei0rActor frei0rActor = new Frei0rActor(_top, "frei0rTest");
		frei0rActor.frei0rLibraryName.setExpression("wrongLibraryName");
		frei0rActor.fire();
	}
	
	@Test
	public void testSetFrei0rLibraryNameAttribute() throws NameDuplicationException, IllegalActionException
	{
		Frei0rActor frei0rActor = new Frei0rActor(_top, "frei0rTest");
		//TODO: change this to appropriate path, when the plugin architecture will be fixed
		frei0rActor.frei0rLibraryName.setExpression("$CLASSPATH/contrib/Frei0r/plugins/libnois0r.dylib");		
	}
	
	
	@After
	public void tearDown() throws Exception {
	}

}
