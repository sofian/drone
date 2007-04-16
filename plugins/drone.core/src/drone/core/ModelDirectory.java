/* A directory of open models.
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

import java.util.Iterator;
import java.util.List;

import ptolemy.kernel.ComponentEntity;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.InternalErrorException;
import ptolemy.kernel.util.KernelException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.StringAttribute;

//////////////////////////////////////////////////////////////////////////
//// ModelDirectory

/**
 A directory of open models. An instance of this class is contained
 by a Configuration. Each open model is represented by an instance of
 Effigy.  An effigy represents the model data.
 It contains a string attribute named "identifier"
 with a string value that uniquely identifies the model.
 A typical choice (which depend on the configuration)
 is the canonical URL for a MoML file that describes the model.
 An effigy also contains all open instances of Tableau associated
 with the model.

 @author Steve Neuendorffer and Edward A. Lee
 @version $Id: ModelDirectory.java,v 1.37 2005/07/08 19:55:39 cxh Exp $
 @since Ptolemy II 1.0
 @Pt.ProposedRating Yellow (eal)
 @Pt.AcceptedRating Red (cxh)
 @see Configuration
 @see Effigy
 @see Tableau
 */
public class ModelDirectory extends CompositeEntity {
    /** Construct a model directory with the specified container and name.
     *  @param container The configuration that contains this directory.
     *  @param name The name of the directory.
     *  @exception IllegalActionException If the entity cannot be contained
     *   by the proposed container.  This should not be thrown.
     *  @exception NameDuplicationException If the name coincides with
     *   an entity already in the container.
     */
    public ModelDirectory(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);
    }

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** Get the effigy of the model that corresponds to the specified
     *  identifier.
     *  @param identifier The identifier for the model, such as a URL.
     *  @return The effigy for the model, or null if the model is not
     *   in the directory.
     */
    public Effigy getEffigy(String identifier) {
        Iterator entities = entityList(Effigy.class).iterator();

        while (entities.hasNext()) {
            Effigy entity = (Effigy) entities.next();
            StringAttribute id = (StringAttribute) entity
                    .getAttribute("identifier");

            if (id != null) {
                String idString = id.getExpression();

                if (idString.equals(identifier)) {
                    return entity;
                }
            }
        }

        return null;
    }

    ///////////////////////////////////////////////////////////////////
    ////                         protected methods                 ////

    /** Remove the specified entity, and if there are no more models
     *  in the directory, except possibly the configuration, then
     *  remove this directory from its container.
     *  This method should not be used directly.  Call the setContainer()
     *  method of the entity instead with a null argument.
     *  The entity is assumed to be contained by this composite (otherwise,
     *  nothing happens). This does not alter the entity in any way.
     *  This method is <i>not</i> synchronized on the workspace, so the
     *  caller should be.
     *  This class overrides the superclass to check if this composite is
     *  empty, and if so, calls system.exit
     *  @param entity The entity to remove.
     */
    protected void _removeEntity(ComponentEntity entity) {
        super._removeEntity(entity);

        List remainingEntities = entityList(Effigy.class);

        if (remainingEntities.size() == 0) {
            try {
                setContainer(null);
            } catch (KernelException ex) {
                throw new InternalErrorException("Cannot remove directory!");
            }
        } else {
            if (remainingEntities.size() == 1) {
                // Check to see whether what remains is only the configuration.
                Object remaining = remainingEntities.get(0);

                if (remaining instanceof PtolemyEffigy) {
                    if (((PtolemyEffigy) remaining).getModel() instanceof Configuration) {
                        try {
                            setContainer(null);
                        } catch (KernelException ex) {
                            throw new InternalErrorException(
                                    "Cannot remove directory!");
                        }
                    }
                }
            }

            // Finally, we might have a case where none of the effigies in
            // the application have a tableau, in which case the application
            // no longer has a UI.  If this happens, then we want to remove
            // the directory, triggering the application to exit.
            boolean anyTableau = false;

            // Check to see if the remaining effigies have any tableaux.
            for (Iterator effigies = remainingEntities.iterator(); effigies
                    .hasNext()
                    && !anyTableau;) {
                Effigy effigy = (Effigy) effigies.next();

                if (effigy.numberOfOpenTableaux() > 0) {
                    anyTableau = true;
                }
            }

            // If we can't find any tableau for any of the effigies, then exi
            if (!anyTableau) {
                try {
                    // This gets reentrant...  Ugh..
                    for (Iterator effigies = remainingEntities.iterator(); effigies
                            .hasNext();) {
                        Effigy effigy = (Effigy) effigies.next();
                        effigy.setContainer(null);
                    }
                } catch (KernelException ex) {
                    throw new InternalErrorException("Cannot remove directory!");
                }
            }
        }
    }
}
