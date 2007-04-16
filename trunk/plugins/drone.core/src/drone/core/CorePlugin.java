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
package drone.core;


import org.java.plugin.boot.Application;
import org.java.plugin.boot.ApplicationPlugin;
import org.java.plugin.util.ExtendedProperties;

public final class CorePlugin extends ApplicationPlugin {
    /**
     * This plug-in ID.
     */
    public static final String PLUGIN_ID = "drone.core";

	@Override
	protected Application initApplication(ExtendedProperties arg0, String[] arg1) throws Exception {
		// TODO Auto-generated method stub
		return new CoreApplication(arg0.getProperty("basePath"), arg1, this);
	}

	@Override
	protected void doStart() throws Exception {
		// TODO Auto-generated method stub
		System.out.println("START");
	}

	@Override
	protected void doStop() throws Exception {
		// TODO Auto-generated method stub
		System.out.println("STOP");
	}

}
