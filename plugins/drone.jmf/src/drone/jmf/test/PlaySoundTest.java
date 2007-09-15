package drone.jmf.test;

import ptolemy.actor.Manager;
import ptolemy.actor.TypedCompositeActor;
import ptolemy.data.BooleanToken;
import ptolemy.domains.sdf.kernel.SDFDirector;
import drone.jmf.actor.PlaySound;
import junit.framework.TestCase;

public class PlaySoundTest extends TestCase {
	
	static final String AUDIO_CLIP_URL = "$CLASSPATH/plugins/drone.jmf/data/test.wav";

	public final void testCloneAndConstruction() {
		
		try {
			TypedCompositeActor top = new TypedCompositeActor();
			top.setManager(new Manager("manager"));
			PlaySound playSound = new PlaySound(top, "test");
			playSound.fileNameOrURL.setExpression(AUDIO_CLIP_URL);
			PlaySound copy = (PlaySound)playSound.clone(top.workspace());
		} catch (Exception e) {
			fail("Exception catched: " + e.toString());
		}
	}
	
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
			PlaySound playSound = new PlaySound(top, "test");
			playSound.fileNameOrURL.setExpression(AUDIO_CLIP_URL);

			top.getManager().startRun();
		} catch (Exception e) {
			fail("Exception catched: " + e.toString());
		}
		
	}

}
