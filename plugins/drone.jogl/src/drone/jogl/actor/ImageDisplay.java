/* Displays an image or texture to the screen using JOGL.
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
package drone.jogl.actor;

import java.awt.DisplayMode;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Image;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
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

import drone.jogl.data.TextureToken;
import drone.jogl.util.JOGLUtils;

import ptolemy.actor.lib.Sink;
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
public class ImageDisplay extends Sink implements GLEventListener, KeyListener {
	
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
		
		fullscreen = new Parameter(this, "fullscreen");
		fullscreen.setTypeEquals(BaseType.BOOLEAN);
		fullscreen.setExpression("false");
		
		doubleBuffered = new Parameter(this, "doubleBuffered");
		doubleBuffered.setTypeEquals(BaseType.BOOLEAN);
		doubleBuffered.setExpression("true");

		_displayOnTexture = true;
		_fullscreen = false;
		
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
    	} else if (attribute == fullscreen) {
    		_fullscreen = ((BooleanToken)fullscreen.getToken()).booleanValue();
    		if (_fullscreen) {
				if (!GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice().isFullScreenSupported())
					throw new IllegalActionException(this, "Fullscreen not supported on default screen device.");
    		}
    	} else if (attribute == doubleBuffered) {
    		_doubleBuffered = ((BooleanToken)doubleBuffered.getToken()).booleanValue();
    	} else
    		super.attributeChanged(attribute);
    }

    /** Initialize this actor.
	 *  If place has not been called, then create a frame to display the
	 *  image in.
	 *  @exception IllegalActionException If a contained method throws it.
	 */
	public void initialize() throws IllegalActionException {
		super.initialize();
		
		// Initialize variables.
		_imageWidth = 320;
		_imageHeight = 240;
		
	    _glu = new GLU();
	    
		// This has to be done in the Swing event thread.
		Runnable doDisplay = new Runnable() {
			public void run() {
				_createOrShowWindow();
			}
		};
		
		SwingUtilities.invokeLater(doDisplay);
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
	    if (_displayOnTexture) {
	        gl.glClearColor(0, 0, 0, 0);
	        gl.glEnable(GL.GL_DEPTH_TEST);
	    } else {
		    gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		    gl.glShadeModel(GL.GL_FLAT);
		    gl.glPixelStorei(GL.GL_UNPACK_ALIGNMENT, 1);
	    }
	}
	
	public void display(GLAutoDrawable drawable) {
	    GL gl = drawable.getGL();
	    if (_displayOnTexture) {
	    	if (_textureData != null) {
	    		if (_texture == null)
	    			_texture = TextureIO.newTexture(_textureData);
	    		else
	    			_texture.updateImage(_textureData);
	    		JOGLUtils.drawSquareTexture(gl, _texture);
	    		if (_doubleBuffered)
	    			drawable.swapBuffers();
	    	}
	    } else {
	    	JOGLUtils.drawPixelsFromBuffer(gl, _imageBuffer, _imageWidth, _imageHeight);
	    	if (_doubleBuffered)
	    		drawable.swapBuffers();
	    }
	}
	
	public void reshape(GLAutoDrawable drawable, int x, int y, int width, int height) {
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
		if (_canvas == null)
			return;
		
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
	    		throw new InternalErrorException("Input is not an ImageToken nor TextureToken. It is: " + in.getClass().toString());	    		
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
	    		} else {
	    			System.out.println("Image is null");
	    		}
	    	} else {
	    		throw new InternalErrorException("Input is not an ImageToken. It is: " + in.getClass().toString());
	    	}
	    	
	    }
	    
	}
	
	private void _resizeCanvas(int newWidth, int newHeight) {
		if (_canvas != null && _frame != null &&
				(_imageWidth != newWidth || _imageHeight != newHeight)) {
			_imageWidth = newWidth;
			_imageHeight = newHeight;
			if (!_fullscreen) {
				_canvas.setSize(_imageWidth, _imageHeight);
				_frame.pack();
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////
	////                         private methods                   ////
	
	/** Create or show the top-level window, unless there already is a
	 *  container. This must be called in the Swing event thread.
	 */
	private void _createOrShowWindow() {
	    // Create canvas and display mode.
		if (_doubleBuffered) {
			GLCapabilities caps = new GLCapabilities();
			caps.setDoubleBuffered(true);
			_canvas = new GLCanvas(caps);
		} else
			_canvas = new GLCanvas();
	    _canvas.addGLEventListener(this);
	    _canvas.setVisible(true);
	    
	    // Create frame if not exists.
	    if (_frame == null)
	    {
	    	_frame = new JFrame();
	    	_frame.getContentPane().add(_canvas);
	    	_frame.addKeyListener(this);
	    }	    

		// Initialize screen (fullscreen/windowed).
		_initScreen();
	}

	/**
	 * Performs screen initialization (allows switches between fullscreen and windowed styles).
	 * From http://forum.java.sun.com/thread.jspa?threadID=235247&messageID=2701575
	 */
	private void _initScreen() {
		try {
			GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice(); 
			if (_fullscreen) {
				_frame.dispose();
				_frame.setUndecorated(true);
				_frame.setResizable(false);
				gd.setFullScreenWindow(_frame);
				_frame.setVisible(true);
				_frame.toFront();
			} else {
				_frame.dispose();
				_frame.setUndecorated(false);
				_frame.setSize(_imageWidth, _imageHeight);
				gd.setFullScreenWindow(null);
				_frame.setVisible(true);
				_frame.toFront();
			}
		} catch (Exception ex) {
			throw new InternalErrorException(ex);
		}
	}
	
	///////////////////////////////////////////////////////////////////
	////                         protected variables               ////
	
	/** The container for the image display, set by calling place(). */
	//protected Container _container;
	
	public Parameter displayOnTexture;
	public Parameter fullscreen;
	public Parameter doubleBuffered;
	
	private boolean _displayOnTexture;
	private boolean _fullscreen;
	private boolean _doubleBuffered;
	
	protected JFrame _frame;
	protected GLCanvas _canvas;
	
	protected Texture _texture = null;
	protected TextureData _textureData = null;
	
	private Buffer _imageBuffer = null;
	
	private int _imageWidth;
	private int _imageHeight;
	
	private GLU _glu;

	/**
	 * The enter key allows to switch from fullscreen <-> windowed. Escape key allows
	 * to get out of fullscreen.
	 */ 
	public void keyPressed(KeyEvent event) {
		// TODO: There seems to be a focus problem. If you click on the window when it's in
		// fullcreen mode the key events aren't sent anymore.
		if (event.getKeyCode() == KeyEvent.VK_ENTER ||
				_fullscreen && event.getKeyCode() == KeyEvent.VK_ESCAPE) {
			_fullscreen = !_fullscreen;
			_initScreen();
		}
	}

	public void keyReleased(KeyEvent event) {
		
	}

	public void keyTyped(KeyEvent event) {
	}
	
}
