/* Load a datasource from a URL.
 * 
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
package drone.jmf.actor;

import java.net.URL;

import javax.media.Manager;
import javax.media.protocol.DataSource;

import ptolemy.actor.Director;
import ptolemy.actor.lib.Source;
import ptolemy.actor.util.Time;
import ptolemy.data.ObjectToken;
import ptolemy.data.expr.FileParameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

//////////////////////////////////////////////////////////////////////////
//// StreamLoader

/** Load a datasource from a URL.
 <p> This actor generates an ObjectToken that contains a DataSource.
 <p> This actor is typically connected to a {@link VideoPlayer} actor.

 @author James Yeh
 @version $Id: StreamLoader.java,v 1.28 2006/08/21 23:11:49 cxh Exp $
 @since Ptolemy II 4.0
 @Pt.ProposedRating Red (cxh)
 @Pt.AcceptedRating Red (cxh)
 */
public class StreamLoader extends Source {
    /** Construct an actor with the given container and name.
     *  @param container The container.
     *  @param name The name of this actor.
     *  @exception IllegalActionException If the actor cannot be contained
     *   by the proposed container.
     *  @exception NameDuplicationException If the container already has an
     *   actor with this name.
     */
    public StreamLoader(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);
        output.setTypeEquals(BaseType.OBJECT);
        fileOrURL = new FileParameter(this, "fileOrURL");
    }

    /** The file or URL that it to be read.
     */
    public FileParameter fileOrURL;

    public void initialize() throws IllegalActionException {
        super.initialize();

        URL url = fileOrURL.asURL();

        if (url == null) {
            throw new IllegalActionException("URLToken was null");
        } else {
            try {
                _dataSource = Manager.createDataSource(url);
            } catch (Exception error) {
                throw new IllegalActionException("Invalid URL");
            }
        }

        Director director = getDirector();

        if (director != null) {
            director.fireAt(this, new Time(director));
        } else {
            throw new IllegalActionException(this, "No director");
        }
    }

    public boolean postfire() throws IllegalActionException {
        if (getDirector().getModelTime().getDoubleValue() == 0.0) {
            output.send(0, new ObjectToken(_dataSource));
        }

        return super.postfire();
    }

    private DataSource _dataSource;
}
