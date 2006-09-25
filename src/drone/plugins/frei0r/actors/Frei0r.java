/* An actor that computes a specified String comparison function on
 the two String inputs.

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

 PT_COPYRIGHT_VERSION_2
 COPYRIGHTENDKEY

 */
package drone.plugins.frei0r.actors;

import ptolemy.actor.TypedAtomicActor;
import ptolemy.actor.TypedIOPort;
import ptolemy.actor.parameters.PortParameter;
import ptolemy.data.BooleanToken;
import ptolemy.data.StringToken;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.InternalErrorException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.Settable;
import ptolemy.kernel.util.StringAttribute;
import ptolemy.kernel.util.Settable.Visibility;
import java.io.*;

import drone.plugins.frei0r.Frei0rException;

// ////////////////////////////////////////////////////////////////////////
// // StringCompare

/**
 * Provide a bridge for Frei0r, minimalistic plugin API for video effects.
 * http://www.piksel.org/frei0r/1.0/spec/index.html
 * 
 * 
 * 
 * @author Mathieu Guindon
 */
public class Frei0r extends TypedAtomicActor {

	/**
	 * Construct an actor with the given container and name. Construct the two
	 * operand input PortParameters (initialized to "") and the output port
	 * which outputs the result of the various comparison functions executed by
	 * the actor. The function to be executed is decided by the parameter
	 * <i>function</i>, which is also initialized here to the comparison
	 * function equals. The <i>ignoreCase</i> parameter allows to ignore case
	 * when comparing.
	 * 
	 * @param container
	 *            The container.
	 * @param name
	 *            The name of this actor.
	 * @exception IllegalActionException
	 *                If the actor cannot be contained by the proposed
	 *                container.
	 * @exception NameDuplicationException
	 *                If the container already has an actor with this name.
	 */
	public Frei0r(CompositeEntity container, String name)
			throws NameDuplicationException, IllegalActionException {
		super(container, name);

		frei0rLibraryName = new StringAttribute(this, "frei0rLibraryName");
		frei0rLibraryName.setVisibility(Settable.NOT_EDITABLE);

		_attachText("_iconDescription", "<svg>\n"
				+ "<rect x=\"-30\" y=\"-15\" " + "width=\"60\" height=\"30\" "
				+ "style=\"fill:white\"/>\n" + "</svg>\n");
	}

	protected void finalize() {
		f0r_destruct(_jniInstance);
		f0r_deinit();
		closeLibrary();
	}

	private void loadFrei0rLibrary() throws Frei0rException {
		String frei0rLibName = frei0rLibraryName.getExpression();

		openLibrary(frei0rLibName);

		f0r_init();
		/*
		 * if (nativeError!=1) throw new Frei0rException("frei0r library " +
		 * frei0rLibName + " fail to init"); //TODO: dynamic image size
		 * _jniInstance = f0r_construct(100, 100);
		 * 
		 * if (_jniInstance==0) throw new Frei0rException("frei0r library " +
		 * frei0rLibName + " fail to construct");
		 */
	}

	public void attributeChanged(Attribute attribute)
			throws IllegalActionException {

		if (attribute == frei0rLibraryName) {
			try {
				loadFrei0rLibrary();
			} catch (Exception e) {
				throw new IllegalActionException(e.getMessage());
			}
		} else {
			super.attributeChanged(attribute);
		}
	}

	// /////////////////////////////////////////////////////////////////
	// // JNI section ////

	static {
		System.loadLibrary("Frei0rJNI");
	}

	/**
	 * Used by native code exclusively. Handle on the frei0r struct in the
	 * native code.
	 */
	@SuppressWarnings("unused")
	private int _handle;

	private native void openLibrary(String libName);

	private native void closeLibrary();

	private native int f0r_init();

	private native void f0r_deinit();

	private native int f0r_construct(int width, int height);

	private native void f0r_destruct(int instance);

	private native void f0r_update(int instance, double time, int[] inframe,
			int[] outframe);

	// /////////////////////////////////////////////////////////////////
	// // ports and parameters ////

	/**
	 * The frei0r dynamic library(.so, .jnilib, .dll) that the actor will
	 * expose.
	 */
	public StringAttribute frei0rLibraryName;

	// /////////////////////////////////////////////////////////////////
	// // public methods ////

	/**
	 * Consume exactly one input token from each input port, and compute the
	 * specified string function of the input taking into account the
	 * <i>ignoreCase</i> parameter.
	 * 
	 * @exception IllegalActionException
	 *                If there is no director.
	 */
	public void fire() throws IllegalActionException {
		super.fire();

	}

	// /////////////////////////////////////////////////////////////////
	// // private variables ////
	private int _jniInstance;

}
