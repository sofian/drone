/* Displays replacement textures on top of detected markers.
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
package drone.artkp.actor;

import java.awt.Font;
import java.awt.Image;
import java.nio.FloatBuffer;
import java.util.Map;

import drone.artkp.util.ARTKPUtils;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLCanvas;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.Threading;
import javax.media.opengl.glu.GLU;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import com.sun.opengl.util.GLUT;
import com.sun.opengl.util.texture.Texture;
import com.sun.opengl.util.texture.TextureCoords;
import com.sun.opengl.util.texture.TextureData;
import com.sun.opengl.util.texture.TextureIO;

import ptolemy.actor.TypedAtomicActor;
import ptolemy.actor.TypedIOPort;
import drone.jogl.actor.ImageDisplay;
import drone.jogl.util.JOGLUtils;
import drone.jogl.data.TextureToken;
import ptolemy.data.ArrayToken;
import ptolemy.data.ImageToken;
import ptolemy.data.IntToken;
import ptolemy.data.ObjectToken;
import ptolemy.data.RecordToken;
import ptolemy.data.Token;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.BaseType;
import ptolemy.data.type.RecordType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.InternalErrorException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.Settable;
import ptolemy.kernel.util.Workspace;
import ptolemy.media.Picture;


//////////////////////////////////////////////////////////////////////////
////ARTKPDisplay

/**
 Display an image on the screen using the ptolemy.media.Picture
 class.  For a sequence of images that are all the same size, this class
 will continually update the picture with new data.   If the size of the
 input image changes, then a new Picture object is created.  This class
 will only accept an ImageToken on its input.
 
 @author James Yeh, Edward A. Lee
 @version $Id: ImageDisplay.java,v 1.39.2.2 2005/05/04 15:51:26 cxh Exp $
 @since Ptolemy II 3.0
 @Pt.ProposedRating Red
 @Pt.AcceptedRating Red
 */
@SuppressWarnings("serial")
public class ARTKPDisplay extends TypedAtomicActor implements GLEventListener {

	public static final int DUMMY_ID = -1;
	
	/** Construct an actor with the given container and name.
	 *  @param container The container.
	 *  @param name The name of this actor.
	 *  @exception IllegalActionException If the actor cannot be contained
	 *   by the proposed container.
	 *  @exception NameDuplicationException If the container already has an
	 *   actor with this name.
	 */
	public ARTKPDisplay(CompositeEntity container, String name)
	throws IllegalActionException, NameDuplicationException {
		super(container, name);
		
		imageIn = new TypedIOPort(this, "imageIn", true, false);
		imageIn.setTypeEquals(BaseType.OBJECT);

		trackerInfoIn = new TypedIOPort(this, "trackerInfo", true, false);
		trackerInfoIn.setTypeEquals(ARTKPActor.TRACKER_INFO_TYPE);

		textureReplacements = new TypedIOPort(this, "textureReplacements", true, false);
		textureReplacements.setTypeEquals(BaseType.OBJECT);
		
//		// FIXME: This is required to be an ImageToken, but
//		// we don't see to have that class.
//		textureIn = new TypedIOPort(this, "textureIn", true, false);
//		textureIn.setTypeEquals(BaseType.OBJECT);
		
		_frame = null;
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
		ARTKPDisplay newObject = (ARTKPDisplay) super.clone(workspace);
		// TODO: implement the clone method the right way
		newObject._frame = null;
		newObject._canvas = null;
		newObject._imageTextureData = null;
		
		return newObject;
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
	    _glut = new GLUT();
	}
	
	public void finalize() {
		if (_texture != null)
			_texture.dispose();
	}
	
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
		if (imageIn.getWidth() > 0 && imageIn.hasToken(0)) {
			Token img = imageIn.get(0);
			if (imageIn.hasToken(0))
				throw new IllegalActionException(this, "image has too many tokens");
			if (img == null)
				throw new IllegalActionException("Got null token");
			
			if (img instanceof ImageToken) {
	    		
	    		Image image = ((ImageToken)img).asAWTImage();
	    		if (image != null) {
	    			_resizeCanvas(image.getWidth(null), image.getHeight(null));
	    			
	    			// XXX For a strange reason that I have yet to understand, I could not
	    			// just call JOGLUtils.drawPixelsFromImage in the display() method since
	    			// it was generating an error under linux. It sounds like a thread issue
	    			// tough I'm far from being sure about that.
	    			_imageTextureData = JOGLUtils.createTextureDataFromImage(image);
	    		} else {
	    			System.out.println("Image is null");
	    		}

	    	} else if (img instanceof TextureToken) {
	    		_imageTextureData = ((TextureToken)img).getTextureData();
    			_resizeCanvas(_imageTextureData.getWidth(), _imageTextureData.getHeight());    			
	    	} else {
	    		throw new InternalErrorException("Input is not an ImageToken nor TextureToken. It is: " + img);	    		
	    	}
			
			// Hack: ensure theres no superposition of phantom markers
			_projectionMatrix = null;
			_markerIDs = null;
			_textureReplacements = null;

		}
		if (trackerInfoIn.getWidth() > 0 && trackerInfoIn.hasToken(0)) {
			Token in = trackerInfoIn.get(0);
			RecordToken record = (RecordToken)in;
			_projectionMatrix = (float[]) ((ObjectToken)record.get("projectionMatrix")).getValue();
			Token[] tokenMarkerIDs = ((ArrayToken)record.get("markerIds")).arrayValue();
			Token[] tokenMarkerMatrix = ((ArrayToken)record.get("markerMatrix")).arrayValue();
			_markerIDs = new int[tokenMarkerIDs.length];
			_markerMatrix = new float[tokenMarkerMatrix.length][];
			for (int i=0; i<_markerIDs.length; i++) {
				_markerIDs[i] = ((IntToken)tokenMarkerIDs[i]).intValue();
				_markerMatrix[i] = (float[]) ((ObjectToken)tokenMarkerMatrix[i]).getValue();
			}
		}
		if (textureReplacements.getWidth() > 0 && textureReplacements.hasToken(0)) {
			Token in = textureReplacements.get(0);
			_textureReplacements = (TextureData[])((ObjectToken)in).getValue();
		}
		if (_canvas != null)
			_canvas.display();
	}
	
	/** Consume a token from the <i>input</i> port
	 *  and display the token as an image.  If a token is not available,
	 *  do nothing.
	 *  @exception IllegalActionException If there is no director, or
	 *   if the base class throws it.
	 */
	public boolean postfire() throws IllegalActionException {
		
		return super.postfire();
	}
	
	synchronized public void init(GLAutoDrawable drawable) {
	    GL gl = drawable.getGL();
	    gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	    gl.glShadeModel(GL.GL_FLAT);
	    gl.glPixelStorei(GL.GL_UNPACK_ALIGNMENT, 1);
	}
	
	public void display(GLAutoDrawable drawable) {
	    GL gl = drawable.getGL();
		gl.glMatrixMode(GL.GL_PROJECTION);
//	    gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT); // clear the buffers for new image
		gl.glLoadIdentity();
		_glu.gluOrtho2D(0, 1, 0, 1);
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadIdentity();
		
		// Display image.
	    if (_imageTextureData != null) {
//			ARTKPUtils.argDrawMode2D(gl, _imageWidth, _imageHeight);
//			gl.glMatrixMode(GL.GL_MODELVIEW);
//			gl.glLoadIdentity();
			if (_texture == null)
				_texture = TextureIO.newTexture(_imageTextureData);
			else
				_texture.updateImage(_imageTextureData);
	    	JOGLUtils.drawSquareTexture(gl, _texture);
	    }
	    // FIXME: this part is a big hack
	    if (_markerIDs != null && _markerMatrix != null && _textureReplacements != null) {
	    	if (_markerIDs.length != _markerMatrix.length || _markerIDs.length != _textureReplacements.length) {
	    		throw new Error("Lengths differ: " + _markerIDs.length + " " + _markerMatrix.length + " " + _textureReplacements.length);
	    	} else {
	    	for (int i=0; i<_markerIDs.length; i++)
	    		_drawMarker(gl, _markerIDs[i], _textureReplacements[i], _markerMatrix[i]);
	    	}
	    }
	    // does not seem to be useful
	    drawable.swapBuffers();
	}
	
	public void reshape(GLAutoDrawable drawable, int x, int y, int width, int height) {
		System.out.println("Reshape");
		GL gl = drawable.getGL();
		_windowWidth = width;
		_windowHeight = height;
//		ARTKPUtils.argDrawMode2D(gl, _windowWidth, _windowHeight);
		System.out.println("done.");
//		GL gl = drawable.getGL();
//		JOGLUtils.rescale(gl, _glu, _imageWidth, _imageHeight, width, height);
	}
	
	public void displayChanged(GLAutoDrawable drawable, boolean modeChanged, boolean deviceChanged) {
	}
	
	/** Display the specified token. This must be called in the Swing
	 *  event thread.
	 *  @param in The token to display
	 */
	protected void _display(Token in) {
    
	}

	private void _drawMarker(GL gl, int markerID, TextureData replacement, float[] markerMatrix) {
		if (markerID == DUMMY_ID)
			return; // do nothing
		
	    float[]   mat_ambient     = {0.0f, 0.0f, 1.0f, 1.0f};
	    float[]   mat_flash       = {0.0f, 0.0f, 1.0f, 1.0f};
	    float[]   mat_flash_shiny = {50.0f};
	    float[]   light_position  = {100.0f,-200.0f,200.0f,0.0f};
	    float[]   ambi            = {0.1f, 0.1f, 0.1f, 0.1f};
	    float[]   lightZeroColor  = {0.9f, 0.9f, 0.9f, 0.1f};
		
		ARTKPUtils.argDrawMode3D(gl);
		// TODO: comprendre pourquoi cette ligne est fautive
		// ARTKPUtils.argDraw3dCamera(gl, 0, 0, _windowWidth, _windowHeight, _imageWidth, _imageHeight, _projectionMatrix);
		// Ces deux lignes remplacent le call a argDraw3dCamera pcq pour le moment ca fout la merde
	    gl.glMatrixMode(GL.GL_PROJECTION);
	    gl.glLoadMatrixf( _projectionMatrix, 0 );
	    
	    gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadMatrixf(markerMatrix, 0);
		gl.glClearDepth(1.0);
	    gl.glClear(GL.GL_DEPTH_BUFFER_BIT);
	    gl.glEnable(GL.GL_DEPTH_TEST);
	    gl.glDepthFunc(GL.GL_LEQUAL);
	    
	    gl.glEnable(GL.GL_LIGHTING);
	    gl.glEnable(GL.GL_LIGHT0);
	    gl.glLightfv(GL.GL_LIGHT0, GL.GL_POSITION, FloatBuffer.wrap(light_position));
	    gl.glLightfv(GL.GL_LIGHT0, GL.GL_AMBIENT, FloatBuffer.wrap(ambi));
	    gl.glLightfv(GL.GL_LIGHT0, GL.GL_DIFFUSE, FloatBuffer.wrap(lightZeroColor));
	    gl.glMaterialfv(GL.GL_FRONT, GL.GL_SPECULAR, FloatBuffer.wrap(mat_flash));
	    gl.glMaterialfv(GL.GL_FRONT, GL.GL_SHININESS, FloatBuffer.wrap(mat_flash_shiny));	
	    gl.glMaterialfv(GL.GL_FRONT, GL.GL_AMBIENT, FloatBuffer.wrap(mat_ambient));
	    gl.glMatrixMode(GL.GL_MODELVIEW);
	    
	    _texture.updateImage(replacement);
//	  draw
		{
		    _texture.enable();
			_texture.bind();
		    
			gl.glTexEnvi(GL.GL_TEXTURE_ENV, GL.GL_TEXTURE_ENV_MODE, GL.GL_REPLACE);
			TextureCoords coords = _texture.getImageTexCoords();
			
			gl.glBegin(GL.GL_QUADS);
			float DIMENSION = 40.0f;
			// A droite
//			float x0 = DIMENSION * 0.5f;
//			float x1 = x0 + 3*DIMENSION;
//			float y0 = - DIMENSION * 0.5f;
//			float y1 = y0 + DIMENSION;
			// En bas
			float x0 = - 0.5f * DIMENSION;
			float width = DIMENSION;
			float height = 0.25f * DIMENSION;
			float y0 = - (0.5f * DIMENSION + 1.5f * height);
			float y1 = y0 + height;
			float x1 = x0 + width;
			gl.glTexCoord2f(coords.left(), coords.bottom());
			gl.glVertex3f(x0, y0, 0);
			gl.glTexCoord2f(coords.right(), coords.bottom());
			gl.glVertex3f(x1, y0, 0);
			gl.glTexCoord2f(coords.right(), coords.top());
			gl.glVertex3f(x1, y1, 0);
			gl.glTexCoord2f(coords.left(), coords.top());
			gl.glVertex3f(x0, y1, 0);
			gl.glEnd();
			
			_texture.disable();
		}
//	    _drawText(gl, "insensible");
//	    _drawMarkerID(gl, markerID);
		gl.glDisable(GL.GL_DEPTH_TEST);
	}
		
//	private void _drawText(GL gl, String text) {
//		TextureData textData = JOGLUtils.createTextureDataFromText(text, new Font("Helvetica", Font.PLAIN, 36), 4*64, 64);
//		Texture texture = TextureIO.newTexture(textData);
//		// draw
//		{
//		    texture.enable();
//			texture.bind();
//		    
//			gl.glTexEnvi(GL.GL_TEXTURE_ENV, GL.GL_TEXTURE_ENV_MODE, GL.GL_REPLACE);
//			TextureCoords coords = texture.getImageTexCoords();
//			
//			gl.glBegin(GL.GL_QUADS);
//			float DIMENSION = 20.0f;
//			float x0 = DIMENSION * 0.5f;
//			float x1 = x0 + 3*DIMENSION;
//			float y0 = - DIMENSION * 0.5f;
//			float y1 = y0 + DIMENSION;
//			gl.glTexCoord2f(coords.left(), coords.bottom());
//			gl.glVertex3f(x0, y0, 0);
//			gl.glTexCoord2f(coords.right(), coords.bottom());
//			gl.glVertex3f(x1, y0, 0);
//			gl.glTexCoord2f(coords.right(), coords.top());
//			gl.glVertex3f(x1, y1, 0);
//			gl.glTexCoord2f(coords.left(), coords.top());
//			gl.glVertex3f(x0, y1, 0);
//			gl.glEnd();
//			
//			texture.disable();
//		}
//		texture.dispose();
//	}
//	
//	private void _drawMarkerID(GL gl, int markerID) {
//	    gl.glDisable(GL.GL_TEXTURE_2D);
//	    int font = GLUT.BITMAP_HELVETICA_18;
//	    gl.glRasterPos2i(0,0);
//	    _glut.glutBitmapString(font, Integer.toString(markerID));
//	    gl.glEnable(GL.GL_TEXTURE_2D);		
//	}
	
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
	
	/** The frame, if one is used. */
	protected JFrame _frame;
	
	protected GLCanvas _canvas;
	
	protected Texture _texture = null;
	
	protected TextureData _imageTextureData = null;

	public TypedIOPort imageIn;
	
	public TypedIOPort trackerInfoIn;
	
	public TypedIOPort textureReplacements;
	
	public TypedIOPort algorithm;
	
	private float[] _projectionMatrix = null;
	private float[][] _markerMatrix = null;
	private int[] _markerIDs = null;
	
	private TextureData[] _textureReplacements = null;
	
	int _nMarkers = -1;
	
	private int _markerId = -1;
	
	private int _imageWidth = 0;
	private int _imageHeight = 0;
	private int _windowWidth = 0;
	private int _windowHeight = 0;
	
	private GLU _glu;
	private GLUT _glut;
	
	/** The picture panel. */
	protected Picture _picture;
	
}
