
import ptolemy.actor.Manager;
import ptolemy.actor.TypedCompositeActor;
import ptolemy.actor.TypedIOPort;
import ptolemy.actor.lib.Const;
import ptolemy.actor.lib.string.StringLength;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.domains.sdf.kernel.SDFDirector;


public class SpikePerformance {
	
	static final int NB_ACTORS = 500;
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {

			TypedCompositeActor top = new TypedCompositeActor();
			top.setName("Overhead performance test");
			Manager manager = new Manager("Manager");
			top.setManager(manager);
			SDFDirector sdfDirector = new SDFDirector(top, "SDFDirector");
	
			sdfDirector.iterations.setExpression("3000");

			Const actorIntConst = new Const(top, "intconst");
//			actorIntConst.value.setExpression("10");
			
			StringLength[] actorStringLength = new StringLength[NB_ACTORS];
			actorStringLength[0] = new StringLength(top, "strlen");

			top.connect((TypedIOPort) actorStringLength[0].input,
					(TypedIOPort) actorIntConst.output);

			for (int i = 1; i < NB_ACTORS; i++) {
				actorStringLength[i] = new StringLength(top, "strlen" + i);

				top.connect((TypedIOPort) actorStringLength[i].input,
						(TypedIOPort) actorStringLength[i-1].output);
			}
			
			long startTimeMs = System.currentTimeMillis();
			top.getManager().run();
			long endTimeMs = System.currentTimeMillis();
			System.out.println("for " + NB_ACTORS + " actors, execution time is " + (endTimeMs - startTimeMs) + "ms");
			

		} catch (NameDuplicationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalActionException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
}
