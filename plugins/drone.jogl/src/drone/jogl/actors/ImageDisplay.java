/* Display a java.awt.Image
 
 @Copyright (c) 1998-2005 The Regents of the University of California.
 All rights reserved.
 
 Permission is hereby granted, without written agreement and without
 license or royalty fees, to use, copy, modify, and distribute this
 software and its documentation for any purpose, provided that the
 above copyright notice and the following two paragraphs appear in all
 copies of this software.
 
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
 
 PT_COPYRIGHT_VERSION 2
 COPYRIGHTENDKEY
 */
package ptolemy.actor.lib.artkp.jogl;


import java.awt.Image;
import java.nio.Buffer;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLCanvas;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.Threading;
import javax.media.opengl.glu.GLU;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import com.sun.opengl.util.texture.Texture;
import com.sun.opengl.util.texture.TextureData;
import com.sun.opengl.util.texture.TextureIO;

import ptolemy.actor.lib.Sink;
import ptolemy.actor.lib.artkp.jogl.TextureToken;
import ptolemy.data.BooleanToken;
import ptolemy.data.ImageToken;
import ptolemy.data.Token;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.InternalErrorException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.Workspace;


//////////////////////////////////////////////////////////////////////////
////ImageDisplay

/**
 Display an image on the screen using JOGL.
 Uses the technique described here:
 http://today.java.net/pub/a/today/2003/09/11/jogl2d.html
 
 This class will only accept an ImageToken on its input.
 
 @author James Yeh, Edward A. Lee
 @version $Id: ImageDisplay.java,v 1.39.2.2 2005/05/04 15:51:26 cxh Exp $
 @since Ptolemy II 3.0
 @Pt.ProposedRating Red
 @Pt.AcceptedRating Red
 */
@SuppressWarnings("serial")
public class ImageDisplay extends Sink implements GLEventListener {
	
	/** Construct an actor with the given container and name.
	 *  @param container The container.
	 *  @param name The name of this actor.
	 *  @exception IllegalActionException If the actor cannot be contained
	 *   by the proposed container.
	 *  @exception NameDuplicationException If the container already has an
	 *   actor with this name.
	 */
	public ImageDisplay(CompositeEntity container, String name)
	throws IllegalActionException, NameDuplicationException {
		super(container, name);

		input.setTypeEquals(BaseType.OBJECT);

		displayOnTexture = new Parameter(this, "displayOnTexture");
		displayOnTexture.setTypeEquals(BaseType.BOOLEAN);
		displayOnTexture.setExpression("true");
		
		_frame = null;
	}
	
	public void finalize() {
		if (_texture != null)
			_texture.dispose();
	}

	///////////////////////////////////////////////////////////////////
	////                         public methods                    ////
	
	/** Clone the actor into the specified workspace. This calls the
	 *  base class and then removes association with graphical objects
	 *  belonging to the original class.
	 *  @param workspace The workspace for the new object.
	 *  @return A new actor.
	 *  @exception CloneNotSupportedException If a derived class contains
	 *   an attribute that cannot be cloned.
	 */
	public Object clone(Workspace workspace)
	throws CloneNotSupportedException {
		ImageDisplay newObject = (ImageDisplay) super.clone(workspace);
		// TODO: implement the clone method the right way
		newObject._frame = null;
		newObject._canvas = null;
		newObject._texture = null;
		newObject._textureData = null;
		
		return newObject;
	}
	
	 /** 
     */
    public void attributeChanged(Attribute attribute)
            throws IllegalActionException {
    	if (attribute == displayOnTexture) {
    		_displayOnTexture = ((BooleanToken)displayOnTexture.getToken()).booleanValue();
    	}
    }

    /** Initialize this actor.
	 *  If place has not been called, then create a frame to display the
	 *  image in.
	 *  @exception IllegalActionException If a contained method throws it.
	 */
	public void initialize() throws IllegalActionException {
		super.initialize();
		
		// This has to be done in the Swing event thread.
		Runnable doDisplay = new Runnable() {
			public void run() {
				_createOrShowWindow();
			}
		};
		
		SwingUtilities.invokeLater(doDisplay);

	    _glu = new GLU();
	}
	
	
	/** Consume a token from the <i>input</i> port
	 *  and display the token as an image.  If a token is not available,
	 *  do nothing.
	 *  @exception IllegalActionException If there is no director, or
	 *   if the base class throws it.
	 */
	public boolean postfire() throws IllegalActionException {
		if (input.getWidth() > 0 && input.hasToken(0)) {
			final Token in = input.get(0);
			if (in == null)
				throw new IllegalActionException("Got null token");
			
			// Display probably to be done in the Swing event thread.
			Runnable doDisplay = new Runnable() {
				public void run() {
					_display(in);
				}
			};

			Threading.invokeOnOpenGLThread(doDisplay);
		}
		
		return super.postfire();
	}
	
	synchronized public void init(GLAutoDrawable drawable) {
	    GL gl = drawable.getGL();
	    gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	    gl.glShadeModel(GL.GL_FLAT);
	    gl.glPixelStorei(GL.GL_UNPACK_ALIGNMENT, 1);
	}
	
	public void display(GLAutoDrawable drawable) {
//		System.out.println("display - repaint - called");
	    GL gl = drawable.getGL();
	    if (_displayOnTexture) {
	    	if (_textureData != null) {
	    		if (_texture == null)
	    			_texture = TextureIO.newTexture(_textureData);
	    		else
	    			_texture.updateImage(_textureData);
	    		JOGLUtils.drawSquareTexture(gl, _texture);
	    	}
	    } else {
	    	JOGLUtils.drawPixelsFromBuffer(gl, _imageBuffer, _imageWidth, _imageHeight);
	    }
	}
	
	public void reshape(GLAutoDrawable drawable, int x, int y, int width, int height) {
//		System.out.println("reshaped called");
		GL gl = drawable.getGL();
	    if (_displayOnTexture) {
	    	JOGLUtils.rescale(gl, _glu, _imageWidth, _imageHeight, width, height);
	    } else {
	    	JOGLUtils.drawPixelsRescale(gl, _glu, _imageWidth, _imageHeight, width, height);
	    }
	}
	
	public void displayChanged(GLAutoDrawable drawable, boolean modeChanged, boolean deviceChanged) {
	}
	
	/** Display the specified token. This must be called in the Swing
	 *  event thread.
	 *  @param in The token to display
	 */
	protected void _display(Token in) {
//		System.out.println("_display called");
	    if (_displayOnTexture) {
	    	
	    	if (in instanceof ImageToken) {
	    		
	    		Image image = ((ImageToken)in).asAWTImage();
	    		if (image != null) {
	    			_resizeCanvas(image.getWidth(null), image.getHeight(null));
	    			
	    			// XXX For a strange reason that I have yet to understand, I could not
	    			// just call JOGLUtils.drawPixelsFromImage in the display() method since
	    			// it was generating an error under linux. It sounds like a thread issue
	    			// tough I'm far from being sure about that.
	    			_textureData = JOGLUtils.createTextureDataFromImage(image);
	    			_canvas.display();
	    		} else {
	    			System.out.println("Image is null");
	    		}

	    	} else if (in instanceof TextureToken) {
	    		_textureData = ((TextureToken)in).getTextureData();
    			_resizeCanvas(_textureData.getWidth(), _textureData.getHeight());    			
    			_canvas.display();
	    	} else {
	    		throw new InternalErrorException("Input is not an ImageToken nor TextureToken. It is: " + in);	    		
	    	}

	    	
	    } else {
	    	
	    	if (in instanceof ImageToken) {
	    		
	    		Image image = ((ImageToken)in).asAWTImage();
	    		if (image != null) {
	    			_resizeCanvas(image.getWidth(null), image.getHeight(null));

	    			// XXX For a strange reason that I have yet to understand, I could not
	    			// just call JOGLUtils.drawPixelsFromImage in the display() method since
	    			// it was generating an error under linux. It sounds like a thread issue
	    			// tough I'm far from being sure about that.
	    			_imageBuffer = JOGLUtils.createPixelDrawingBufferFromImage(image);
	    			_canvas.display();
//	    			_canvas.repaint();
	    		} else {
	    			System.out.println("Image is null");
	    		}
	    	} else {
	    		throw new InternalErrorException("Input is not an ImageToken. It is: " + in);
	    	}
	    	
	    }
	    
	}
	
	private void _resizeCanvas(int newWidth, int newHeight) {
		if (_imageWidth != newWidth || _imageHeight != newHeight) {
			_imageWidth = newWidth;
			_imageHeight = newHeight;
			_canvas.setSize(_imageWidth, _imageHeight);
			_frame.pack();
		}
	}
	
	///////////////////////////////////////////////////////////////////
	////                         private methods                   ////
	
	/** Create or show the top-level window, unless there already is a
	 *  container. This must be called in the Swing event thread.
	 */
	private void _createOrShowWindow() {
		if (_frame == null) {
			// No current container for the pane.
			try {
				_frame = new JFrame();
				
				_frame.setVisible(true);
			} catch (Exception ex) {
				throw new InternalErrorException(ex);
			}
		} else {
			if (_frame != null) {
				// Do not use show() as it overrides manual placement.
				_frame.toFront();
			}
		}
		
	    /*
	     * display mode (single buffer and RGBA)
	     */
	    GLCapabilities caps = new GLCapabilities();
	    caps.setDoubleBuffered(false);
		_canvas = new GLCanvas(caps);
	    _canvas.addGLEventListener(this);
	    _canvas.setVisible(true);
	    
		if (_frame != null) {
			_frame.add(_canvas);
		    _frame.setSize(300, 300);
			_frame.setVisible(true);
			_frame.toFront();
		}
	}

	///////////////////////////////////////////////////////////////////
	////                         protected variables               ////
	
	/** The container for the image display, set by calling place(). */
	//protected Container _container;
	
	public Parameter displayOnTexture;
	
	private boolean _displayOnTexture;
	
	/** The frame, if one is used. */
	protected JFrame _frame;
	
	protected GLCanvas _canvas;
	
	protected Texture _texture = null;
	
	protected TextureData _textureData = null;
	
	private Buffer _imageBuffer = null;
	
	private int _imageWidth = 0;
	private int _imageHeight = 0;
	
	private GLU _glu;
	
}
