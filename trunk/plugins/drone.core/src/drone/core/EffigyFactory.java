/* An object that can create a new Effigy
 *
 * Copyright (c) 1998-2006 The Regents of the University of California.
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

import java.net.URL;
import java.util.Iterator;

import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.Workspace;

//////////////////////////////////////////////////////////////////////////
//// EffigyFactory

/**
 A configuration contains an instance of this class, and uses it to create
 effigies from a URL, or to create blank effigies of a particular kind.
 This base class assumes that it contains other effigy factories.
 Its createEffigy() methods defer to each contained factory in order
 until one is capable of creating an effigy. Subclasses of this class
 will usually be inner classes of an Effigy and will create the Effigy,
 or they might themselves be aggregates of instances of EffigyFactory.

 @author Steve Neuendorffer and Edward A. Lee
 @version $Id: EffigyFactory.java,v 1.26 2005/07/08 19:55:31 cxh Exp $
 @since Ptolemy II 1.0
 @Pt.ProposedRating Yellow (eal)
 @Pt.AcceptedRating Red (cxh)
 @see Configuration
 @see Effigy
 */
public class EffigyFactory extends CompositeEntity {
    /** Create a factory in the specified workspace.
     *  @param workspace The workspace.
     */
    public EffigyFactory(Workspace workspace) {
        super(workspace);
    }

    /** Create a factory with the given name and container.
     *  @param container The container.
     *  @param name The name.
     *  @exception IllegalActionException If the container is incompatible
     *   with this entity.
     *  @exception NameDuplicationException If the name coincides with
     *   an entity already in the container.
     */
    public EffigyFactory(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);
    }

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** Return true if this effigy factory is capable of creating
     *  an effigy without a URL being specified.  That is, it is capable
     *  of creating a blank effigy with no model data.
     *  In this base class, this method returns true if at least one
     *  contained effigy factory returns true.
     *  @return True if this factory can create a blank effigy.
     */
    public boolean canCreateBlankEffigy() {
        Iterator factories = entityList(EffigyFactory.class).iterator();

        while (factories.hasNext()) {
            EffigyFactory factory = (EffigyFactory) factories.next();

            if (factory.canCreateBlankEffigy()) {
                return true;
            }
        }

        return false;
    }

    /** Create a new blank effigy in the given container. This base class
     *  defers to each contained effigy factory until one returns
     *  an effigy.  If there are no contained effigies, or if none
     *  returns an effigy, then this method returns null. Subclasses will
     *  override this method to create an effigy of an appropriate type.
     *  @param container The container for the effigy.
     *  @return A new effigy.
     *  @exception Exception If the effigy created by one of the contained
     *   factories is incompatible with the specified container, or a name
     *   duplication occurs.
     */
    public Effigy createEffigy(CompositeEntity container) throws Exception {
        return createEffigy(container, null, null);
    }

    /** Create a new effigy in the given container by reading the specified
     *  URL. If the specified URL is null, then create a blank effigy.
     *  The specified base is used to expand any relative file references
     *  within the URL.  This base class defers to each contained effigy
     *  factory until one returns an effigy.  If there are no
     *  contained effigies, or if none
     *  returns an effigy, then this method returns null. Subclasses will
     *  override this method to create an effigy of an appropriate type.
     *  @param container The container for the effigy.
     *  @param base The base for relative file references, or null if
     *   there are no relative file references.
     *  @param in The input URL.
     *  @return A new effigy.
     *  @exception Exception If the stream cannot be read, or if the data
     *   is malformed in some way.
     */
    public Effigy createEffigy(CompositeEntity container, URL base, URL in)
            throws Exception {
        Effigy effigy = null;
        Iterator factories = entityList(EffigyFactory.class).iterator();

        while (factories.hasNext() && (effigy == null)) {
            EffigyFactory factory = (EffigyFactory) factories.next();
            effigy = factory.createEffigy(container, base, in);
        }

        return effigy;
    }

    /** Return the extension on the name of the specified URL.
     *  This is a utility method designed to help derived classes
     *  decide whether the URL matches the particular type of effigy
     *  they can create.  If the URL has no extension, return an
     *  empty string.
     *  @param url A URL.
     *  @return The extension on the URL.
     */
    public static String getExtension(URL url) {
        String filename = url.getFile();
        int index = filename.lastIndexOf(".");

        if (index < 0) {
            return "";
        }

        try {
            return filename.substring(index + 1);
        } catch (IndexOutOfBoundsException ex) {
            return "";
        }
    }
}
