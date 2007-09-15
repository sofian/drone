/* An actor that provides a bridge to the Frei0r video effect API.
 * 
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
package drone.frei0r.actor;

import ptolemy.actor.TypedAtomicActor;
import ptolemy.actor.TypedIOPort;
import ptolemy.actor.parameters.ParameterPort;
import ptolemy.actor.parameters.PortParameter;
import ptolemy.data.AWTImageToken;
import ptolemy.data.BooleanToken;
import ptolemy.data.DoubleToken;
import ptolemy.data.ImageToken;
import ptolemy.data.IntToken;
import ptolemy.data.ScalarToken;
import ptolemy.data.StringToken;
import ptolemy.data.Token;
import ptolemy.data.expr.FileParameter;
import ptolemy.data.expr.Parameter;
import ptolemy.data.expr.StringParameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.Port;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.InternalErrorException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.Settable;
import ptolemy.kernel.util.StringAttribute;
import ptolemy.kernel.util.Settable.Visibility;

import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.*;
import java.net.URL;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import drone.frei0r.Frei0rException;
import drone.frei0r.jni.Frei0r;
import drone.util.ImageConvert;
import drone.util.OSUtils;



//Frei0rActor

/**
 * Provides a bridge for Frei0r, minimalistic plugin API for video effects.
 * http://www.piksel.org/frei0r/1.0/spec/index.html
 * 
 * @author Mathieu Guindon, Jean-Sebastien Senecal
 */
public class Frei0rActor extends TypedAtomicActor {

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
	public Frei0rActor(CompositeEntity container, String name)
	throws NameDuplicationException, IllegalActionException {
		super(container, name);

		frei0rLibraryName = new StringParameter(this, "frei0rLibraryName");
		frei0rLibraryName.setVisibility(Settable.NOT_EDITABLE);

		input1 = new TypedIOPort(this, "input1", true, false);
		input1.setTypeEquals(BaseType.OBJECT);

		output = new TypedIOPort(this, "output", false, true);
		output.setTypeEquals(BaseType.OBJECT);

		defaultWidth = new Parameter(this, "defaultWidth");
		defaultWidth.setTypeEquals(BaseType.INT);
		defaultWidth.setExpression("320");

		defaultHeight = new Parameter(this, "defaultHeight");
		defaultHeight.setTypeEquals(BaseType.INT);
		defaultHeight.setExpression("320");

		params = new Vector<PortParameter>();

		_attachText("_iconDescription", "<svg>\n"
				+ "<rect x=\"-30\" y=\"-15\" " + "width=\"60\" height=\"30\" "
				+ "style=\"fill:white\"/>\n" + "</svg>\n");
	}

	public void initialize() throws IllegalActionException {
		try {

			if (_frei0r != null)
				_frei0rInstance = _frei0r.createInstance(((IntToken)defaultWidth.getToken()).intValue(), 
						((IntToken)defaultHeight.getToken()).intValue());

		} catch (Frei0rException e) {
			throw new IllegalActionException(this, e, "Problem when creating instance while initializing the actor.");
		} catch (Exception e) {
			throw new IllegalActionException(this, e, "Problem during initialization.");
		}
	}

	public void attributeChanged(Attribute attribute) throws IllegalActionException {

		if (attribute == frei0rLibraryName) {

			try {
				String newLibraryName = ((StringToken)frei0rLibraryName.getToken()).stringValue();
				
				if (_frei0r == null || _currentLibraryName.equals(newLibraryName)) {
					String newLibraryFileName = newLibraryName + "." + OSUtils.getNativeLibraryExtension();
					String frei0rDirName = "frei0r-" + Frei0r.FREI0R_MAJOR_VERSION;
					_createFrei0r("/usr/lib/" + frei0rDirName + "/" + newLibraryFileName);
					_createFrei0r("/usr/local/lib/" + frei0rDirName + "/" + newLibraryFileName);
					_createFrei0r("$HOME/." + frei0rDirName + "/" + newLibraryFileName);
					_createFrei0r(newLibraryName); // try the old method, with complete path specified
					if (_frei0r == null)
						throw new IllegalActionException(this, "Specified frei0r library '" + newLibraryName + 
						"' could not be found or loaded");

					// This ensures that we don't reload for nothing
					_currentLibraryName = newLibraryName;

					// Create bridge.
					_frei0rInstance = _frei0r.createInstance(((IntToken)defaultWidth.getToken()).intValue(), 
							((IntToken)defaultHeight.getToken()).intValue());					

					// Reuse ports if they already exist.
					if (getPort("input2") != null)
						input2 = (TypedIOPort) getPort("input2");

					if (getPort("input3") != null)
						input3 = (TypedIOPort) getPort("input3");

					// Remove them if unneeded.
					if (input2 != null &&
							_frei0r.getPluginType() != Frei0r.F0R_PLUGIN_TYPE_MIXER2 &&
							_frei0r.getPluginType() != Frei0r.F0R_PLUGIN_TYPE_MIXER3) {
						input2.setContainer(null);
						_removePort(input2);
					}
					if (input3 != null && _frei0r.getPluginType() != Frei0r.F0R_PLUGIN_TYPE_MIXER3) {
						input3.setContainer(null);
						_removePort(input2);
					}

					// Create inputs 2 and 3 if necessary.
					switch (_frei0r.getPluginType()) {
					case Frei0r.F0R_PLUGIN_TYPE_MIXER3:
						if (input3 == null) {
							input3 = new TypedIOPort(this, "input3", true, false);
							input3.setTypeEquals(BaseType.OBJECT);
						}
					case Frei0r.F0R_PLUGIN_TYPE_MIXER2:
						if (input2 == null) {
							input2 = new TypedIOPort(this, "input2", true, false);
							input2.setTypeEquals(BaseType.OBJECT);
						}
					default:;
					}

					// Reinitialize attributes for this actor.
//					Iterator it = attributeList().iterator();
//					while (it.hasNext()) {
//					Attribute a = (Attribute) it.next();
//					if (a instanceof PortParameter) {
//					a.setContainer(null);
//					Port p = ((PortParameter)a).getPort();
//					p.setContainer(null);
//					_removePort(p);
//					_removeAttribute(a);
//					params.remove(a);
//					System.out.println("Remove attribute " + a.toString());
//					}
//					}

					if (params.size() == 0) {
						for (int i=0; i<_frei0r.nParams(); ++i) {
							// TODO: check unicity of param name
							PortParameter param;
							String paramName = _frei0r.getParamName(i);
							if (getAttribute(paramName) != null) {
								// Don't re-add the same port.
								// XXX This is a bit of a hack to prevent errors when loading a file
								// We should find a better way to do that
								params.add((PortParameter)getAttribute(paramName));
								continue;
							}
							switch (_frei0r.getParamType(i)) {
							// BOOLEAN
							case Frei0r.F0R_PARAM_BOOL:
								// Add a boolean port.
								param = new PortParameter(this, paramName);
								param.setTypeEquals(BaseType.BOOLEAN);
								param.setExpression("true");
								params.add(param);
								break;
								// DOUBLE
							case Frei0r.F0R_PARAM_DOUBLE:
								// Add a double port.
								param = new PortParameter(this, paramName);
								param.setTypeEquals(BaseType.DOUBLE);
								param.setExpression("0.5");
								params.add(param);
								break;
								// POSITION
							case Frei0r.F0R_PARAM_POSITION:
								// Add a double port for X.
								param = new PortParameter(this, paramName + " (x)");
								param.setTypeEquals(BaseType.DOUBLE);
								param.setExpression("0.5");
								params.add(param);
								// Add a double port for Y.
								param = new PortParameter(this, paramName + " (y)");
								param.setTypeEquals(BaseType.DOUBLE);
								param.setExpression("0.5");
								params.add(param);
								break;
								// COLOR
							case Frei0r.F0R_PARAM_COLOR:
								// Add a double port for R.
								param = new PortParameter(this, paramName + " (r)");
								param.setTypeEquals(BaseType.DOUBLE);
								param.setExpression("1.0");
								params.add(param);
								// Add a double port for G.
								param = new PortParameter(this, paramName + " (g)");
								param.setTypeEquals(BaseType.DOUBLE);
								param.setExpression("0.0");
								params.add(param);
								// Add a double port for B.
								param = new PortParameter(this, paramName + " (b)");
								param.setTypeEquals(BaseType.DOUBLE);
								param.setExpression("0.0");
								params.add(param);
								break;
								// STRING
							case Frei0r.F0R_PARAM_STRING:
								// Add a string port.
								param = new PortParameter(this, paramName);
								param.setTypeEquals(BaseType.STRING);
								param.setExpression("");
								params.add(param);
								break;
							default:
								throw new IllegalActionException("Wrong param type " + _frei0r.getParamType(i) +
								", please verify the frei0r plugin.");
							}
						}
					} 
				} 
			}  catch (NameDuplicationException e) {
				throw new IllegalActionException(this, e, "Trying to use an already existing port parameter.");
			} catch (Exception e) {
				throw new IllegalActionException(this, e, "Problem at library name change.");
			} 
		} else {
			super.attributeChanged(attribute);
		}
	}

	// /////////////////////////////////////////////////////////////////
	// // ports and parameters ////

	/**
	 * The frei0r dynamic library(.so, .jnilib, .dll) that the actor will
	 * expose.
	 */
	public StringParameter frei0rLibraryName;

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

		// Call update on each param, so that they consume their associated port's token.
		// This is required, otherwise a ptolemy.actor.NoRoomException will be thrown.
		for (int i=0; i<params.size(); i++) {
			params.get(i).update();
		}

		try {

			// Process image.
			BufferedImage bufferedImageIn1 = null;
			BufferedImage bufferedImageIn2 = null;
			BufferedImage bufferedImageIn3 = null;

			boolean hasInput = true;

			switch (_frei0r.getPluginType()) {
			case Frei0r.F0R_PLUGIN_TYPE_MIXER3:
				if (input3.getWidth() > 0 && input3.hasToken(0)) {
					bufferedImageIn3 = _getInputImageFromToken(input3.get(0));
					if (bufferedImageIn3 == null)
						hasInput = false;
				} else
					hasInput = false; 
			case Frei0r.F0R_PLUGIN_TYPE_MIXER2:
				if (input2.getWidth() > 0 && input2.hasToken(0)) {
					bufferedImageIn2 = _getInputImageFromToken(input2.get(0));
					if (bufferedImageIn2 == null)
						hasInput = false;
					else if (bufferedImageIn3 != null && 
							(bufferedImageIn3.getWidth() != bufferedImageIn2.getWidth() ||
									bufferedImageIn3.getHeight() != bufferedImageIn2.getHeight())) {
						throw new IllegalActionException(this, "Image inputs have incompatible dimensions.");
					}
				} else
					hasInput = false; 
			case Frei0r.F0R_PLUGIN_TYPE_FILTER:
				if (input1.getWidth() > 0 && input1.hasToken(0)) {
					bufferedImageIn1 = _getInputImageFromToken(input1.get(0));
					if (bufferedImageIn1 == null)
						hasInput = false;
					else if (bufferedImageIn2 != null && 
							(bufferedImageIn2.getWidth() != bufferedImageIn1.getWidth() ||
									bufferedImageIn2.getHeight() != bufferedImageIn1.getHeight())) {
						throw new IllegalActionException(this, "Image inputs have incompatible dimensions.");
					}
				} else
					hasInput = false; 
			}

			// If there is no image input, do nothing.
			// NOTICE: Even sources thus need to have an input, the which acts as a trigger.
			if (hasInput) {
				// Create instance or resize it.
				if (_frei0rInstance == null ||
						_frei0rInstance.getWidth() != bufferedImageIn1.getWidth() ||
						_frei0rInstance.getHeight() != bufferedImageIn1.getHeight()) {
					_frei0rInstance = _frei0r.createInstance(bufferedImageIn1.getWidth(), bufferedImageIn1.getHeight());
				}

				// Process params.
				int k = 0;
				for (int i=0; i<_frei0r.nParams(); i++) {
					PortParameter paramPort;
					Object paramValue = null;
					switch (_frei0r.getParamType(i)) {
					// BOOLEAN
					case Frei0r.F0R_PARAM_BOOL:
						paramPort = params.get(k++);
						paramValue = new Boolean( ((BooleanToken)paramPort.getToken()).booleanValue() );
						break;
						// DOUBLE
					case Frei0r.F0R_PARAM_DOUBLE:
						paramPort = params.get(k++);
						paramValue = new Double( ((ScalarToken)paramPort.getToken()).doubleValue() );
						break;
						// POSITION
					case Frei0r.F0R_PARAM_POSITION:
						Frei0r.Position positionValue = (Frei0r.Position)_frei0rInstance.getParamValue(i);
						if (positionValue == null) {
							positionValue = new Frei0r.Position(0.0, 0.0);
						}
						// X
						paramPort = params.get(k++);
						positionValue.setX( ((DoubleToken)paramPort.getToken()).doubleValue() );
						// Y
						paramPort = params.get(k++);
						positionValue.setY( ((DoubleToken)paramPort.getToken()).doubleValue() );
						// Assign.
						paramValue = positionValue;
						break;
						// COLOR
					case Frei0r.F0R_PARAM_COLOR:
						Frei0r.Color colorValue = (Frei0r.Color)_frei0rInstance.getParamValue(i);
						if (colorValue == null) {
							colorValue = new Frei0r.Color(0.0f, 0.0f, 0.0f);
						}
						// RED
						paramPort = params.get(k++);
						colorValue.setRed( (float) ((DoubleToken)paramPort.getToken()).doubleValue() );
						// GREEN
						paramPort = params.get(k++);
						colorValue.setGreen( (float) ((DoubleToken)paramPort.getToken()).doubleValue() );
						// BLUE
						paramPort = params.get(k++);
						colorValue.setBlue( (float) ((DoubleToken)paramPort.getToken()).doubleValue() );
						// Assign.
						paramValue = colorValue;
						break;
						// STRING
					case Frei0r.F0R_PARAM_STRING:
						paramPort = params.get(k++);
						paramValue = ((StringToken)paramPort.getToken()).stringValue();
						break;
					default:
						throw new IllegalActionException("Wrong param type " + _frei0r.getParamType(i) +
						", please verify the frei0r plugin.");
					}

					// Set the value.
					if (paramValue != null)
						_frei0rInstance.setParamValue(paramValue, i);
				}

				// Update.
				BufferedImage bufferedImageOut = 
					new BufferedImage(_frei0rInstance.getWidth(), 
							_frei0rInstance.getHeight(),
							BufferedImage.TYPE_INT_ARGB);

				if (_frei0r.getPluginType() == Frei0r.F0R_PLUGIN_TYPE_MIXER2 || 
						_frei0r.getPluginType() == Frei0r.F0R_PLUGIN_TYPE_MIXER3) {
					_frei0rInstance.update2(getDirector().getModelTime().getDoubleValue(), bufferedImageIn1,
							bufferedImageIn2, bufferedImageIn3, bufferedImageOut);
				} else {
					_frei0rInstance.update(getDirector().getModelTime().getDoubleValue(), bufferedImageIn1, bufferedImageOut);
				}
				output.send(0, new AWTImageToken(bufferedImageOut));
			}

		} catch (Frei0rException e) {
			throw new IllegalActionException(this, e, "Processing of image failed.");
		}
	}

	private BufferedImage _getInputImageFromToken(Token token) throws IllegalActionException {
		if (token instanceof ImageToken) {
			Image imageIn = ((ImageToken)token).asAWTImage();
			if (imageIn == null)
				return null;
			else
				return ImageConvert.toBufferedImage(imageIn, BufferedImage.TYPE_INT_ARGB);
		} else
			throw new IllegalActionException(this, "Input has wrong token type " + token.getClass().toString() + 
			"(should be ImageToken).");
	}

	protected void _createFrei0r(String libraryName) {
		if (_frei0r != null)
			return;
		try {
			_frei0r = new Frei0r(libraryName);
		} catch (Frei0rException e) {
		}
	}

	private Frei0r _frei0r = null;
	private Frei0r.Instance _frei0rInstance;
	private String _currentLibraryName = null;

	public TypedIOPort output;

	public TypedIOPort input1;
	public TypedIOPort input2;
	public TypedIOPort input3;

	public Vector<PortParameter> params;
	public Parameter defaultWidth;
	public Parameter defaultHeight;

}
