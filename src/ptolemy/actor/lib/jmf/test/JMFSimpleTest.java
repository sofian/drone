package ptolemy.actor.lib.jmf.test;

import ptolemy.actor.TypedCompositeActor;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.domains.sdf.kernel.SDFDirector;
import ptolemy.actor.lib.jmf.MovieReader;
import ptolemy.actor.lib.jmf.ImageDisplay;

//import ptolemy.kernel.util.Workspace;
//import ptolemy.domains.de.kernel.DEDirector;
//import ptolemy.domains.dt.kernel.DTDirector;
//import ptolemy.domains.ct.kernel.CTDirector;
import ptolemy.actor.Manager;
//import ptolemy.actor.lib.StringConst;
//import ptolemy.actor.lib.Writer;

//import java.awt.*;
//import java.awt.event.*;
//import java.awt.image.*;

public class JMFSimpleTest {

	public static void main(String[] args) throws NameDuplicationException, IllegalActionException {
		
			TypedCompositeActor top = new TypedCompositeActor();
			
			// Create the director.
			SDFDirector director = new SDFDirector(top, "director");
			String period = Double.toString(1.0 / 27.0);
			director.period.setExpression(period);
			director.synchronizeToRealTime.setExpression("true");
			director.iterations.setExpression("100");
			Manager manager = new Manager("manager");
			top.setManager(manager);
			
			// Create two actors.
			MovieReader reader = new MovieReader(top, "reader");
			ImageDisplay writer = new ImageDisplay(top, "display");
			reader.fileOrURL.setExpression("/Users/tats/Documents/workspace/drone/src/ptolemy/actor/lib/jmf/MrPtolemy.mov");
			top.connect(reader.output, writer.input);
			
//			top.getManager().startRun();
			top.getManager().run();
	}
}
