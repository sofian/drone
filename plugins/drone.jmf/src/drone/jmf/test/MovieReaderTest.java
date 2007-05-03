/*
 * Copyright (c) 2003-2006 The Regents of the University of California.
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
package drone.jmf.test;

import ptolemy.actor.Manager;
import ptolemy.actor.TypedCompositeActor;
import ptolemy.data.BooleanToken;
import ptolemy.domains.sdf.kernel.SDFDirector;

import junit.framework.TestCase;

import drone.jmf.actor.ImageDisplay;
import drone.jmf.actor.MovieReader;

public class MovieReaderTest extends TestCase {

	static final String MOVIE_URL = "http://drone.ws/media/TravelAgent.mov";
	
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
