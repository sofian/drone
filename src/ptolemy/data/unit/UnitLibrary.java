/* The Unit Library

Copyright (c) 2003-2005 The Regents of the University of California.
All rights reserved.
Permission is hereby granted, without written agreement and without
license or royalty fees, to use, copy, modify, and distribute this
software and its documentation for any purpose, provided that the above
copyright notice and the following two paragraphs appear in all copies
of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS.

PT_COPYRIGHT_VERSION_3
COPYRIGHTENDKEY
*/
package ptolemy.data.unit;

import java.net.URL;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;
import java.util.Vector;

import ptolemy.data.expr.Parameter;
import ptolemy.kernel.util.InternalErrorException;
import ptolemy.kernel.util.NamedObj;
import ptolemy.moml.MoMLParser;
import ptolemy.util.JNLPUtilities;

//////////////////////////////////////////////////////////////////////////
//// UnitLibrary

/** A Library containing definitions of commonly used units.

Currently, the Unit library is static in the sense that it is
loaded when the system starts and is hard-wired to a particular
Unit System (the System International Unit System). However, it
should be easy to change the architecture so that multiple Unit
Systems can be accommodated, and loaded on-the-fly.

@author Rowland R Johnson
@version $Id: UnitLibrary.java,v 1.26 2005/04/29 20:04:56 cxh Exp $
@since Ptolemy II 4.0
@Pt.ProposedRating Red (rowland)
@Pt.AcceptedRating Red (rowland)
*/
public class UnitLibrary {
    public UnitLibrary() {
    }

    ///////////////////////////////////////////////////////////////////
    ////                         public variables                  ////

    /* The Identity Unit, i.e. 1.0*<0, 0, ..., 0> */
    public static final Unit Identity;

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** Add a unit to the Library.
     * @param unit Unit to add to the library.
     */
    public static void addToLibrary(Unit unit) {
        _unitsLibrary.add(unit);
    }

    /** Find the Unit in the library that is basic (scale equal to 1),
     * singular (all but one dimensions equal to 0),  XXXXX
     * @param catNum
     * @return The basic, singular unit.
     */
    public static Unit getBaseUnit(int catNum) {
        Vector library = UnitLibrary.getLibrary();

        for (int i = 0; i < library.size(); i++) {
            Unit lUnit = (Unit) (library.elementAt(i));

            if (lUnit.getScale() != 1.0) {
                continue;
            }

            int[] ltype = lUnit.getType();

            if (ltype[catNum] != 1.0) {
                continue;
            }

            for (int j = 0; j < UnitLibrary.getNumCategories(); j++) {
                if ((j != catNum) && (ltype[j] != 0)) {
                    continue;
                }
            }

            return lUnit;
        }

        return null;
    }

    /** Search Library to find Unit that has the same type and is the closest to
     *  a unit in terms of the scalars.
     * @return The Unit closest to this the argument. Null, if there are no
     * Units in the Library with the same type.
     */
    public static Unit getClosestUnit(Unit unit) {
        Vector possibles = getUnitsByType(unit);

        if (possibles.isEmpty()) {
            return null;
        }

        double scalarDistance = Double.MAX_VALUE;
        Unit retv = null;

        for (int i = 0; i < possibles.size(); i++) {
            Unit possible = (Unit) (possibles.elementAt(i));
            double distance = Math.abs(possible.getScale() - unit.getScale());

            if (distance < scalarDistance) {
                scalarDistance = distance;
                retv = possible;
            }
        }

        return retv;
    }

    /** Return the Library.
     * @return The Library
     */
    public static Vector getLibrary() {
        return _unitsLibrary;
    }

    /** Return the number of categories.
     * @return Number of categories.
     */
    public static int getNumCategories() {
        return _numCats;
    }

    /** Return the Parser.
     * @return The Parser.
     */
    public static UParser getParser() {
        return _parser;
    }

    /** Search Library for Unit equal to a particular unit. That is, both the
     *  type and scalar must be equal to the argument.
     * @return Unit in Library equal to this one. Null if none found.
     */
    public static Unit getUnit(Unit unit) {
        Unit retv = getClosestUnit(unit);

        if (retv == null) {
            return null;
        }

        if (Math.abs(retv.getScale() - unit.getScale()) < 1.0E-8) {
            return retv;
        }

        return null;
    }

    /** Search the Library for a unit with a particular name.
     * @param name The name of the desired unit.
     * @return The unit with name equal to the argument. Null, if the Library
     * doesn't have a unit with that name.
     */
    public static Unit getUnitByName(String name) {
        Vector library = getLibrary();

        for (int i = 0; i < library.size(); i++) {
            Unit lUnit = (Unit) (library.elementAt(i));
            Vector names = lUnit.getLabels();

            for (int j = 0; j < names.size(); j++) {
                if (((String) (names.elementAt(j))).equals(name)) {
                    return lUnit;
                }
            }
        }

        return null;
    }

    /** Search Library for all Units with type equal to this one.
     * @param unit
     * @return Vector of Units with type equal to the argument.
     */
    public static Vector getUnitsByType(Unit unit) {
        Vector retv = new Vector();
        Vector library = UnitLibrary.getLibrary();

        for (int i = 0; i < library.size(); i++) {
            Unit lUnit = (Unit) (library.elementAt(i));

            if (lUnit.hasSameType(unit)) {
                retv.add(lUnit);
            }
        }

        return retv;
    }

    ///////////////////////////////////////////////////////////////////
    ////                         private variables                 ////
    private static boolean _debug = false;
    private static int _numCats;
    private static UParser _parser;
    private static Vector _unitsLibrary;

    static {
        _parser = new UParser();

        MoMLParser momlParser = new MoMLParser();
        UnitSystem us = null;

        try {
            NamedObj container = new NamedObj();
            momlParser.setContext(container);

            // FIXME: There is a bit of a design flaw here because
            // we open a stream to the url (which is probably expensive)
            // and then close it.  The reason for opening the stream
            // is that we want to be sure that the URL is valid,
            // and if it is not, we check the local file system
            // and the classpath.
            // One solution would be to have a method that returned a
            // URLConnection because we can open a stream with a
            // URLConnection and still get the original URL if necessary
            URL inURL = null;
            String spec = "ptolemy/data/unit/SI.xml";
            
            // NOTE: This part of the code was stripped out of ptolemy.actor.gui.MoMLApplication
            // Original call was:
            // URL inURL = MoMLApplication.specToURL("ptolemy/data/unit/SI.xml");
            // We also needed to move JNLPApplication from ptolemy.actor.gui to ptolemy.util

            try {
                // First argument is null because we are only
                // processing absolute URLs this way.  Relative
                // URLs are opened as ordinary files.
                inURL = new URL(null, spec);

                // Make sure that the specURL actually exists
                InputStream urlStream = inURL.openStream();
                urlStream.close();
            } catch (Exception ex) {
                try {
                    // Try as a regular file
                    File file = new File(spec);

                    // Oddly, under Windows file.exists() might return even
                    // though the file does not exist if we changed user.dir.
                    // See
                    // http://forum.java.sun.com/thread.jsp?forum=31&thread=328939
                    // One hack is to convert to an absolute path first
                    File absoluteFile = file.getAbsoluteFile();

                    try {
                        if (!absoluteFile.exists()) {
                            throw new IOException("File '" + absoluteFile
                                    + "' does not exist.");
                        }
                    } catch (java.security.AccessControlException accessControl) {
                        IOException exception = new IOException(
                                "AccessControlException while "
                                        + "trying to read '" + absoluteFile + "'");

                        // IOException does not have a cause argument constructor.
                        exception.initCause(accessControl);
                        throw exception;
                    }

                    inURL = absoluteFile.getCanonicalFile().toURL();

                    //InputStream urlStream = specURL.openStream();
                    //urlStream.close();
                } catch (Exception ex2) {
                    try {
                        // Try one last thing, using the classpath.
                        // Need a class context, and this is a static method, so...
                        // we can't use this.getClass().getClassLoader()
                        // NOTE: There doesn't seem to be any way to convert
                        // this a canonical name, so if a model is opened this
                        // way, and then later opened as a file, the model
                        // directory will think it has two different files.
                        //Class refClass = Class.forName(
                        //        "ptolemy.kernel.util.NamedObj");
                        //specURL = refClass.getClassLoader().getResource(spec);
                        // This works in Web Start, see
                        // http://java.sun.com/products/javawebstart/faq.html#54
                        inURL = Thread.currentThread().getContextClassLoader()
                                .getResource(spec);

                        if (inURL == null) {
                            throw new Exception("getResource(\"" + spec
                                    + "\") returned null.");
                        } else {
                            // If we have a jar URL, convert spaces to %20
                            // so as to avoid multiple windows with the
                            // same file.  Web Start needs this if the Web
                            // Start cache is in a directory that has
                            // spaces in the path, which is the default
                            // under Windows.
                            inURL = JNLPUtilities.canonicalizeJarURL(inURL);

                            // Verify that it can be opened
                            InputStream urlStream = inURL.openStream();
                            urlStream.close();
                        }
                    } catch (Exception ex3) {
                        // Use a very verbose message in case opening
                        // the configuration fails under Web Start.
                        // Without this error message, users will
                        // have no hope of telling us why Web Start failed.
                        IOException exception = new IOException("File not found: '"
                                + spec + "'\n caused by:\n" + ex + "\n AND:\n"
                                + ex2 + "\n AND:\n" + ex3);

                        // IOException does not have a cause argument
                        exception.initCause(ex3);
                        throw exception;
                    }
                }
            }

            // Strangely, the XmlParser does not want as base the
            // directory containing the file, but rather the
            // file itself.
            URL base = inURL;

            momlParser.parse(base, inURL);

            us = (UnitSystem) (container.getAttribute("SI"));
        } catch (Throwable throwable) {
            throw new InternalErrorException(null, throwable,
                    "Failed to initialize statics in UnitLibrary");
        }

        // Initialize the Library.
        _unitsLibrary = new Vector();
        _numCats = UnitUtilities.getNumCategories();
        Identity = new Unit("Identity");
        _unitsLibrary.add(Identity);

        Iterator oldStyleUnits = us.attributeList().iterator();
        UnitLibrary enclosingObject = new UnitLibrary();
        Vector pairs = new Vector();

        while (oldStyleUnits.hasNext()) {
            Object oldStyleUnit = oldStyleUnits.next();

            if (oldStyleUnit instanceof BaseUnit) {
                BaseUnit baseUnit = (BaseUnit) oldStyleUnit;
                Unit basicUnit = new Unit(baseUnit);
                addToLibrary(basicUnit);
            } else if (oldStyleUnit instanceof Parameter) {
                String name = ((Parameter) oldStyleUnit).getName();
                String expr = ((Parameter) oldStyleUnit).getExpression();
                UnitNameExprPair pair = enclosingObject.new UnitNameExprPair(name,
                        expr);
                pairs.add(pair);
            }
        }

        boolean madeChange = true;

        while (!pairs.isEmpty() && madeChange) {
            madeChange = false;

            Iterator iter = pairs.iterator();

            while (iter.hasNext()) {
                UnitNameExprPair pair = (UnitNameExprPair) (iter.next());
                String expr = pair.getUExpr();

                try {
                    UnitExpr uExpr = _parser.parseUnitExpr(expr);
                    Unit unit = uExpr.reduce().getSingleUnit();

                    if (unit != null) {
                        unit.setPrimaryLabel(pair.getName());
                        iter.remove();
                        madeChange = true;
                        addToLibrary(unit);
                    }
                } catch (ParseException e1) {
                    // OK here.
                }
            }
        }

        if (_debug) {
            Vector units = getLibrary();

            for (int i = 0; i < units.size(); i++) {
                System.out.println(((Unit) (units.elementAt(i))).toString());
            }
        }
    }


    /** UnitNameExprPair
     * @author Rowland R Johnson
     * @version $Id: UnitLibrary.java,v 1.26 2005/04/29 20:04:56 cxh Exp $
     * @since Ptolemy II 4.0
     * @Pt.ProposedRating Red (cxh)
     * @Pt.AcceptedRating Red (cxh)
     *
     */
    private class UnitNameExprPair {
        public UnitNameExprPair(String n, String ue) {
            _name = n;
            _uExpr = ue;
        }

        public String getName() {
            return _name;
        }

        public String getUExpr() {
            return _uExpr;
        }

        private String _name;
        private String _uExpr;
    }
}
