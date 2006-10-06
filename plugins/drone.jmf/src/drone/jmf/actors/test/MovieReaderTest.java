package drone.jmf.actors.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import ptolemy.actor.Manager;
import ptolemy.actor.TypedCompositeActor;
import ptolemy.data.BooleanToken;
import ptolemy.domains.sdf.kernel.SDFDirector;

import drone.jmf.actors.ImageDisplay;
import drone.jmf.actors.MovieReader;

public class MovieReaderTest {

	static final String MOVIE_URL = "http://drone.ws/media/TravelAgent.mov";
	
	@Test
	public final void testCloneAndConstruction() {
		
		try {
			TypedCompositeActor top = new TypedCompositeActor();
			top.setManager(new Manager("manager"));
			MovieReader movieReader = new MovieReader(top, "testreader");
			movieReader.fileOrURL.setExpression("");
			MovieReader copy = (MovieReader)movieReader.clone(top.workspace());
			assertEquals( ((BooleanToken)movieReader.isFrameBased.getToken()).booleanValue(), true);
			assertEquals( ((BooleanToken)movieReader.isFrameBased.getToken()).booleanValue(), 
						  ((BooleanToken)copy.isFrameBased.getToken()).booleanValue());
		} catch (Exception e) {
			fail("Exception catched: " + e.toString());
		}
	}
	
	@Test
	public final void testSDF() {
		try {
			TypedCompositeActor top = new TypedCompositeActor();
			
			// Create the director.
			SDFDirector director = new SDFDirector(top, "director");
			String period = Double.toString(1.0 / 27.0);
			director.period.setExpression(period);
			director.synchronizeToRealTime.setExpression("true");
			director.iterations.setExpression("1");
			Manager manager = new Manager("manager");
			top.setManager(manager);

			// Create two actors.
			MovieReader reader = new MovieReader(top, "reader");
			ImageDisplay writer = new ImageDisplay(top, "display");
			reader.fileOrURL.setExpression(MOVIE_URL);
			reader.isFrameBased.setExpression("false");
			top.connect(reader.output, writer.input);

			top.getManager().startRun();
		} catch (Exception e) {
			fail("Exception catched: " + e.toString());
		}
		
	}

}
