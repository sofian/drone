package ptolemy.actor.lib.artkp.demos;

import ptolemy.actor.Director;
import ptolemy.actor.TypedCompositeActor;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.domains.sdf.kernel.SDFDirector;

//import ptolemy.kernel.util.Workspace;
//import ptolemy.domains.de.kernel.DEDirector;
//import ptolemy.domains.dt.kernel.DTDirector;
//import ptolemy.domains.ct.kernel.CTDirector;
import ptolemy.actor.Manager;
import ptolemy.actor.lib.artkp.ARTKPActor;
import ptolemy.actor.lib.image.ImageDisplay;
import ptolemy.actor.lib.image.ImageReader;
import ptolemy.actor.lib.jmf.ImageToJMF;
import ptolemy.actor.lib.jmf.MovieWriter;
//import ptolemy.actor.lib.StringConst;
//import ptolemy.actor.lib.Writer;

//import java.awt.*;
//import java.awt.event.*;
//import java.awt.image.*;

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
			ImageReader reader = new ImageReader(top, "reader");
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
