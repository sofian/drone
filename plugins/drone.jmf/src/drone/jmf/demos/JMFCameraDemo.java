/*
 * Copyright (c) 2006 Jean-Sebastien Senecal (js@drone.ws)
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
package drone.jmf.demos;

import ptolemy.actor.TypedCompositeActor;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.domains.sdf.kernel.SDFDirector;
import drone.jmf.actors.VideoCamera;
import drone.jmf.actors.ImageDisplay;

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

public class JMFCameraDemo {

	public static void main(String[] args) throws NameDuplicationException, IllegalActionException {
		
			TypedCompositeActor top = new TypedCompositeActor();
			
			// Create the director.
			SDFDirector director = new SDFDirector(top, "director");
			String period = Double.toString(1.0 / 27.0);
			director.period.setExpression(period);
			director.synchronizeToRealTime.setExpression("true");
			//director.iterations.setExpression("100");
			Manager manager = new Manager("manager");
			top.setManager(manager);
			top.setDirector(director);
			
			// Create two actors.
			VideoCamera reader = new VideoCamera(top, "reader");
			ImageDisplay writer = new ImageDisplay(top, "display");
			reader.formatName.setExpression("RGB");
//			reader.deviceNumber.setExpression(args[0]);
//			reader.isFrameBased.setExpression("false");
			top.connect(reader.output, writer.input);
			
			top.getManager().startRun();
	}
}
