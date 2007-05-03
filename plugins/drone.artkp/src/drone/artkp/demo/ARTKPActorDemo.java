package drone.artkp.demo;

import ptolemy.actor.Director;
import ptolemy.actor.TypedCompositeActor;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.domains.sdf.kernel.SDFDirector;

import ptolemy.actor.Manager;
import drone.artkp.actor.ARTKPActor;
import drone.jmf.actor.*;

public class ARTKPActorDemo {

	static String DEFAULT_FILE = "/home/tats/eclipse/ptII6.0.alpha/ptolemy/actor/lib/artkp/demos/image_320_240_8_marker_id_simple_nr031.jpg";

	public static void main(String[] args) throws NameDuplicationException, IllegalActionException {
		
			TypedCompositeActor top = new TypedCompositeActor();
			
			// Create the director.
			SDFDirector director = new SDFDirector(top, "director");
			director.iterations.setExpression("10");
			Manager manager = new Manager("manager");
			top.setManager(manager);
			top.setDirector(director);
			
			// Create two actors.
			ARTKPActor actor = new ARTKPActor(top, "artkp");
			MovieReader reader = new MovieReader(top, "reader");
			ImageToJMF transformer = new ImageToJMF(top, "imageToJMF");
			MovieWriter writer = new MovieWriter(top, "writer");
			writer.fileType.setExpression("MPEG");
	        writer.fileOrURL.setExpression("/home/tats/movieOut.mpg");
			reader.fileOrURL.setExpression((args.length > 0 ? args[0] : DEFAULT_FILE));
			top.connect(reader.output, transformer.input);
			top.connect(transformer.output, writer.input);
			
			top.getManager().startRun();
	}
}
