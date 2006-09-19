package ptolemy.actor.lib.jmf.test;

import ptolemy.actor.TypedCompositeActor;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
//import ptolemy.kernel.util.Workspace;
import ptolemy.domains.sdf.kernel.SDFDirector;
import ptolemy.domains.de.kernel.DEDirector;
import ptolemy.domains.dt.kernel.DTDirector;
import ptolemy.domains.ct.kernel.CTDirector;
import ptolemy.actor.Manager;
//import ptolemy.actor.lib.StringConst;
//import ptolemy.actor.lib.Writer;

//import java.awt.*;
//import java.awt.event.*;
//import java.awt.image.*;

public class JMFSimpleTest {

//	public JMFSimpleTest() {
//		MediaTracker mediaTracker = new MediaTracker(this);
//		mediaTracker.addImage(image, 0);
//		try
//		{
//			mediaTracker.waitForID(0);
//		}
//		catch (InterruptedException ie)
//		{
//			System.err.println(ie);
//			System.exit(1);
//		}
//		addWindowListener(new WindowAdapter() {
//      		public void windowClosing(WindowEvent e) {
//        		System.exit(0);
//      		}
//		});
//		setSize(image.getWidth(null), image.getHeight(null));
//		setVisible(true);
//	}
//
//	public void paint(Graphics graphics) {
//		if (!graphics.drawImage(image, 0, 0, this))
//			System.out.println("cannot draw image");
//	}

	public static void main(String[] args) throws NameDuplicationException, IllegalActionException {
		
			TypedCompositeActor top = new TypedCompositeActor();
			
			// Create the director.
			SDFDirector director = new SDFDirector(top, "director");
//			director.iterations.setExpression("0");
//			director.timeResolution.setExpression("1.0");
			DEDirector director = new DEDirector(top, "director");
			director.timeResolution.setExpression("0.037");
			director.startTime.setExpression("0.0");
			director.stopTime.setExpression("10.0");
			director.stopWhenQueueIsEmpty.setExpression("true");
			director.synchronizeToRealTime.setExpression("true");
//			director.period.setExpression("0.037");
			CTDirector director = new CTDirector(top, "director");
			DTDirector director;
//			director.synchronizeToRealTime.setExpression("true");
//			top.setDirector(director);
			//director.stopTime.setExpression("100.0");
			Manager manager = new Manager("manager");
			top.setManager(manager);
			
			// Create two actors.
//			StringConst reader = new StringConst(top, "reader");
//			reader.value.setExpression("hello world");
//			Writer writer = new Writer(top, "writer");
			MovieReader reader = new MovieReader(top, "reader");
//			MovieWriter writer = new MovieWriter(top, "writer");
//			StreamLoader reader = new StreamLoader(top, "loader");
//			VideoPlayer writer = new VideoPlayer(top, "player");
			ImageDisplay writer = new ImageDisplay(top, "display");
//			VideoCamera reader = new VideoCamera(top, "camera");
			reader.fileOrURL.setExpression("/Users/tats/Documents/workspace/drone/src/ptolemy/actor/lib/jmf/MrPtolemy.mov");
//			reader.fileOrURL.setExpression("/Users/tats/Desktop/TravelAgent.mov");
//			reader.fileOrURL.setExpression("/Users/tats/Movies/nivea_visage_3steps.mpg");
//			reader.fileOrURL.setExpression("/Users/tats/Movies/the_scene_xvid_episode_19.avi");
//			writer.fileOrURL.setExpression("/tmp/MovieOut.mov");
			top.connect(reader.output, writer.input);
			
			top.getManager().startRun();
//			top.getManager().run();
	}
}
