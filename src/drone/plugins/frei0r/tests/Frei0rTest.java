package drone.plugins.frei0r.tests;

import org.junit.*;
import ptolemy.actor.TypedCompositeActor;
import drone.plugins.frei0r.actors.*;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

public class Frei0rTest {

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
			new Frei0r(_top, "frei0rTest");
			
		} catch (Exception e) {
			Assert.fail(e.getMessage());
		}
	}
	
	@Test(expected=IllegalActionException.class)
	public void testSetFrei0rLibraryNameAttributeWrong() throws NameDuplicationException, IllegalActionException
	{
		Frei0r frei0rActor = new Frei0r(_top, "frei0rTest");
		frei0rActor.frei0rLibraryName.setExpression("wrongLibraryName");		
	}
	
	@Test
	public void testSetFrei0rLibraryNameAttribute() throws NameDuplicationException, IllegalActionException
	{
		Frei0r frei0rActor = new Frei0r(_top, "frei0rTest");
		frei0rActor.frei0rLibraryName.setExpression("build/drone/plugins/frei0r/contrib/frei0r/plugins/darken.dylib");		
	}
	
	
	@After
	public void tearDown() throws Exception {
	}

}
