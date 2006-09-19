package ptolemy.actor.lib.jmf.test;

import ptolemy.actor.TypedCompositeActor;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.domains.sdf.kernel.SDFDirector;
import ptolemy.actor.Manager;
import ptolemy.actor.lib.StringConst;
import ptolemy.actor.lib.Writer;
import ptolemy.kernel.util.StreamListener;

public class SDFSimpleTest {

	public static void main(String[] args) throws NameDuplicationException, IllegalActionException {
			TypedCompositeActor top = new TypedCompositeActor();
			
			// Create the director.
			SDFDirector director = new SDFDirector(top, "director");
			director.period.setExpression("1");
			director.synchronizeToRealTime.setExpression("true");
//			director.addDebugListener(new StreamListener(System.out));
			Manager manager = new Manager("manager");
			top.setManager(manager);
			
			// Create two actors.
			StringConst reader = new StringConst(top, "reader");
			reader.value.setExpression("hello world");
			Writer writer = new Writer(top, "writer");
			top.connect(reader.output, writer.input);
			
			top.getManager().startRun();
	}
}
