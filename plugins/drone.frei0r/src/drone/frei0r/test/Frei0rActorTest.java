package drone.frei0r.test;

import ptolemy.actor.TypedCompositeActor;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import drone.frei0r.actor.Frei0rActor;

import junit.framework.TestCase;

public class Frei0rActorTest extends TestCase {
	
	private TypedCompositeActor _top;
	
	public void setUp() throws Exception {
		_top = new TypedCompositeActor();
		_top.setName("Frei0r unit tests");
	}

	public void tearDown() throws Exception {
	}

	/**
	 * Nothing specific to test in the constructor, but
	 * the frei0rJNI lib is loaded in a static statement,
	 * so any problems with the native lib will be reported
	 * at the first instanciation.
	 */
	public void testConstructor()
	{
		try
		{
			new Frei0rActor(_top, "frei0rTest");
			
		} catch (Exception e) {
			fail(e.getMessage());
		}
	}
	
	public void testSetFrei0rLibraryNameAttributeWrong() throws NameDuplicationException, IllegalActionException
	{
		Frei0rActor frei0rActor = new Frei0rActor(_top, "frei0rTest");
		frei0rActor.frei0rLibraryName.setExpression("wrongLibraryName");
		frei0rActor.fire();
	}
	
	public void testSetFrei0rLibraryNameAttribute() throws NameDuplicationException, IllegalActionException
	{
		Frei0rActor frei0rActor = new Frei0rActor(_top, "frei0rTest");
		//TODO: change this to appropriate path, when the plugin architecture will be fixed
		frei0rActor.frei0rLibraryName.setExpression("$CLASSPATH/contrib/Frei0r/plugins/libnois0r.dylib");		
	}
		
}
