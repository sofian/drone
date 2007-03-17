package drone.frei0r.actors;

import ptolemy.actor.TypedAtomicActor;
import ptolemy.actor.TypedIOPort;
import ptolemy.actor.parameters.PortParameter;
import ptolemy.data.AWTImageToken;
import ptolemy.data.BooleanToken;
import ptolemy.data.ImageToken;
import ptolemy.data.IntToken;
import ptolemy.data.StringToken;
import ptolemy.data.expr.FileParameter;
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

import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.*;
import java.net.URL;

import drone.frei0r.Frei0rException;
import drone.frei0r.jni.Frei0r;
import drone.util.ImageConvert;


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

		frei0rLibraryName = new FileParameter(this, "frei0rLibraryName");
		frei0rLibraryName.setVisibility(Settable.NOT_EDITABLE);

		input1 = new TypedIOPort(this, "input1", true, false);
		input1.setTypeEquals(BaseType.OBJECT);

		input2 = new TypedIOPort(this, "input2", true, false);
		input2.setTypeEquals(BaseType.OBJECT);

		input3 = new TypedIOPort(this, "input3", true, false);
		input3.setTypeEquals(BaseType.OBJECT);
		
		output = new TypedIOPort(this, "output", false, true);
		output.setTypeEquals(BaseType.OBJECT);

		defaultWidth = new Parameter(this, "defaultWidth");
		defaultWidth.setTypeEquals(BaseType.INT);
		defaultWidth.setExpression("320");
		
		defaultHeight = new Parameter(this, "defaultHeight");
		defaultHeight.setTypeEquals(BaseType.INT);
		defaultHeight.setExpression("320");
		
		_attachText("_iconDescription", "<svg>\n"
				+ "<rect x=\"-30\" y=\"-15\" " + "width=\"60\" height=\"30\" "
				+ "style=\"fill:white\"/>\n" + "</svg>\n");
	}

	public void attributeChanged(Attribute attribute)
			throws IllegalActionException {

		if (attribute == frei0rLibraryName) {
			try {
				if (_frei0r == null) {
					
					_frei0r = new Frei0r(frei0rLibraryName.asURL().getFile());
					_frei0rInstance = _frei0r.createInstance(((IntToken)defaultWidth.getToken()).intValue(), 
															 ((IntToken)defaultHeight.getToken()).intValue());
					
					System.out.println("Instantiating frei0r actor for frei0r lib '" + _frei0r.getName() + "'");
					System.out.println("  Plugin type = " + _frei0r.getPluginType());
					System.out.println("  Color model = " + _frei0r.getColorModel());
					System.out.println("  Num params = " + _frei0r.nParams());
					
					if (_frei0r.getPluginType() != Frei0r.F0R_PLUGIN_TYPE_MIXER2 &&
						_frei0r.getPluginType() != Frei0r.F0R_PLUGIN_TYPE_MIXER3) {
						input2.setContainer(null);
					}
	
					if (_frei0r.getPluginType() != Frei0r.F0R_PLUGIN_TYPE_MIXER3) {
						input3.setContainer(null);
					}
					
				}
				
			} catch (Exception e) {
				throw new IllegalActionException(e.getMessage());
			}
		} else {
			super.attributeChanged(attribute);
		}
	}

	/**
	 * Used by native code exclusively. Handle on the frei0r struct in the
	 * native code.
	 */
	@SuppressWarnings("unused")
	private int _handle;

	// /////////////////////////////////////////////////////////////////
	// // ports and parameters ////

	/**
	 * The frei0r dynamic library(.so, .jnilib, .dll) that the actor will
	 * expose.
	 */
	public FileParameter frei0rLibraryName;

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
		
		try {
			
			BufferedImage bufferedImageIn = null;
			boolean hasInput = false;
			
			if (input1.getWidth() > 0 && input1.hasToken(0)) {
				hasInput = true;
				Image imageIn = ((ImageToken)input1.get(0)).asAWTImage();
				bufferedImageIn = ImageConvert.toBufferedImage(imageIn);
				if (bufferedImageIn.getType() != BufferedImage.TYPE_INT_ARGB) {
					bufferedImageIn = ImageConvert.toARGB(bufferedImageIn);
					
//					BufferedImage tmp = bufferedImageIn;
//					bufferedImageIn = new BufferedImage(bufferedImageIn.getWidth(), bufferedImageIn.getHeight(), 
//													    BufferedImage.TYPE_INT_ARGB);
//					bufferedImageIn.setRGB(0, 0, bufferedImageIn.getWidth(), bufferedImageIn.getHeight(),
//											 tmp.getRGB(0, 0, bufferedImageIn.getWidth(), bufferedImageIn.getHeight(), 
//													    null, 0, bufferedImageIn.getWidth()),
//													    0, bufferedImageIn.getWidth());
				}
			}
			

			// TODO implement support for params
			// If this is a source and there is no image input, put a dummy instead
			if (!hasInput && _frei0r.getPluginType() == Frei0r.F0R_PLUGIN_TYPE_SOURCE) {
				bufferedImageIn = new BufferedImage(_frei0rInstance.getWidth(), _frei0rInstance.getHeight(), 
													BufferedImage.TYPE_INT_ARGB);
			}
			
			// Create instance or resize it.
			if (_frei0rInstance == null ||
				_frei0rInstance.getWidth() != bufferedImageIn.getWidth() ||
				_frei0rInstance.getHeight() != bufferedImageIn.getHeight()) {
				_frei0rInstance = _frei0r.createInstance(bufferedImageIn.getWidth(), bufferedImageIn.getHeight());
			}
			
			// Update.
			if (hasInput || _frei0r.getPluginType() == Frei0r.F0R_PLUGIN_TYPE_SOURCE) {
				
				BufferedImage bufferedImageOut = new BufferedImage(_frei0rInstance.getWidth(), 
																	_frei0rInstance.getHeight(), 
																	BufferedImage.TYPE_INT_ARGB);
				
				if (_frei0r.getPluginType() == Frei0r.F0R_PLUGIN_TYPE_MIXER2 || 
						_frei0r.getPluginType() == Frei0r.F0R_PLUGIN_TYPE_MIXER3) {
					
					BufferedImage bufferedImageIn2 = null;
					BufferedImage bufferedImageIn3 = null;
					if (input2.getWidth() > 0 && input2.hasToken(0)) {
						Image imageIn = ((ImageToken)input2.get(0)).asAWTImage();
						bufferedImageIn2 = ImageConvert.toBufferedImage(imageIn);
					} else {
						bufferedImageIn2 = new BufferedImage(_frei0rInstance.getWidth(), _frei0rInstance.getHeight(), 
								BufferedImage.TYPE_INT_ARGB);
					}
					
					if (_frei0r.getPluginType() == Frei0r.F0R_PLUGIN_TYPE_MIXER3 &&
							input3.getWidth() > 0 && input3.hasToken(0)) {
						Image imageIn = ((ImageToken)input3.get(0)).asAWTImage();
						bufferedImageIn3 = ImageConvert.toBufferedImage(imageIn);
					} else {
						bufferedImageIn3 = new BufferedImage(_frei0rInstance.getWidth(), _frei0rInstance.getHeight(), 
								BufferedImage.TYPE_INT_ARGB);
					}
					_frei0rInstance.update2(getDirector().getModelTime().getDoubleValue(), bufferedImageIn,
											bufferedImageIn2, bufferedImageIn3, bufferedImageOut);
				} else {
					_frei0rInstance.update(getDirector().getModelTime().getDoubleValue(), bufferedImageIn, bufferedImageOut);
				}
				
				// XXX hack for test
				//bufferedImageOut = bufferedImageIn;
				output.send(0, new AWTImageToken(bufferedImageOut));
				
			}
			
		} catch (Frei0rException e) {
			throw new IllegalActionException(this, e.getMessage());
		}
	}

	private Frei0r _frei0r = null;
	private Frei0r.Instance _frei0rInstance;
	
	public TypedIOPort output;
	
	public TypedIOPort input1;
	public TypedIOPort input2;
	public TypedIOPort input3;
	
	public Parameter defaultWidth;
	public Parameter defaultHeight;

}
