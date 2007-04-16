/* An object that can create a tableau for a model.
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

import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.NamedObj;

//////////////////////////////////////////////////////////////////////////
//// TableauFactory

/**
 This class is an attribute that creates a tableau to view a specified effigy.
 When a model is opened, if the top-level of that model contains this
 attribute or a subclass, then that attribute handles constructing a tableau.
 Otherwise, the configuration specifies which tableau is used.
 A configuration contains an instance of this class, and uses it to create
 a tableau for a model represented by an effigy.  This base class assumes
 that it contains other tableau factories. Its createTableau() method defers
 to each contained factory, in the order in which they were added,
 until one is capable of creating a tableau for the specified effigy.
 Subclasses of this class will usually be inner classes of a Tableau,
 and will create the Tableau, or might themselves be aggregates of
 instances of TextEditorTableauFactory.
 <p>
 When there are multiple distinct TableauFactory classes that are capable
 of providing views on the same effigy, then instances of these
 factories should be aggregated into a single factory contained herein.
 Those instances can be presented as alternative views of the data when
 any single view is opened.
 <p>
 There is a significant subtlety with respect to how Ptolemy II classes
 are dealt with. Normally, when one looks inside an instance of a class,
 what is opened is the class definition, not the instance. However,
 if the instance contains an instance of TableauFactory, then what
 is opened is the instance, not the class definition.  This is used,
 for example, when the look inside behavior is customized on a per
 instance basis.

 @author Steve Neuendorffer and Edward A. Lee
 @version $Id: TableauFactory.java,v 1.31 2005/07/08 19:55:48 cxh Exp $
 @since Ptolemy II 1.0
 @Pt.ProposedRating Green (eal)
 @Pt.AcceptedRating Yellow (celaine)
 @see Configuration
 @see Effigy
 @see Tableau
 */
public class TableauFactory extends Attribute {
    /** Create a factory with the given name and container.
     *  @param container The container.
     *  @param name The name.
     *  @exception IllegalActionException If the container is incompatible
     *   with this attribute.
     *  @exception NameDuplicationException If the name coincides with
     *   an attribute already in the container.
     */
    public TableauFactory(NamedObj container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);
    }

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** Create a tableau for the specified effigy. The tableau will be
     *  created with a new unique name with the specified effigy as its
     *  container.  If this factory cannot create a tableau
     *  for the given effigy (perhaps because the effigy is not of the
     *  appropriate subclass), then return null.  This base class assumes
     *  that it contains other tableau factories. This method defers
     *  to each contained factory in order until one is capable of creating a
     *  tableau for the specified effigy.  As with all attributes, the order
     *  is simply the order of creation.  Subclasses of this class will
     *  usually be inner classes of a Tableau, and will create the Tableau.
     *  A subclass that actually creates a tableau is responsible for setting
     *  the container of the tableau to the specified effigy, and for naming
     *  the tableau.
     *  Subclasses should not call show() in createTableau(), it is the
     *  responsibility of the caller to check the return value and
     *  call show() after doing things like adjusting the size.
     *  @param effigy The model effigy.
     *  @return A tableau for the effigy, or null if one cannot be created.
     *  @exception Exception If the factory should be able to create a
     *   Tableau for the effigy, but something goes wrong.
     */
    public Tableau createTableau(Effigy effigy) throws Exception {
        Tableau tableau = null;
        Iterator factories = attributeList(TableauFactory.class).iterator();

        while (factories.hasNext() && (tableau == null)) {
            TableauFactory factory = (TableauFactory) factories.next();
            tableau = factory.createTableau(effigy);
        }

        return tableau;
    }
}
