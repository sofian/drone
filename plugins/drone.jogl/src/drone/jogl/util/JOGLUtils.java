package ptolemy.actor.lib.artkp.jogl;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.color.ColorSpace;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.awt.image.ComponentColorModel;
import java.awt.image.DataBuffer;
import java.awt.image.DataBufferByte;
import java.awt.image.Raster;
import java.awt.image.WritableRaster;
import java.nio.Buffer;
import java.nio.ByteBuffer;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

import com.sun.opengl.util.texture.Texture;
import com.sun.opengl.util.texture.TextureCoords;
import com.sun.opengl.util.texture.TextureData;
import com.sun.opengl.util.texture.TextureIO;

public class JOGLUtils {

    public static TextureData createTextureDataFromImage(Image image) {
		BufferedImage bimage;
		if (image instanceof BufferedImage)
			bimage = (BufferedImage) image;
		else {
			bimage = new BufferedImage (image.getWidth(null),
										image.getHeight(null),
										BufferedImage.TYPE_INT_BGR);
			bimage.createGraphics().drawImage( image, 0, 0, null);
		}
    	return TextureIO.newTextureData(bimage, false);
    }
    
    public static Buffer createPixelDrawingBufferFromImage(Image image) {

    	if (image == null)
    		return null;
    	
    	int width = image.getWidth(null);
    	int height = image.getHeight(null);
    	
		// This is the technique illulstrated in:
		// http://today.java.net/pub/a/today/2003/09/11/jogl2d.html
		WritableRaster raster = 
			Raster.createInterleavedRaster (DataBuffer.TYPE_BYTE,
					width,
					height,
					4,
					null);
		ComponentColorModel colorModel=
			new ComponentColorModel (ColorSpace.getInstance(ColorSpace.CS_sRGB),
					new int[] {8,8,8,8},
					true,
					false,
					ComponentColorModel.TRANSLUCENT,
					DataBuffer.TYPE_BYTE);			
		BufferedImage bimage = 
			new BufferedImage (colorModel,
					raster,
					false,
					null);
		
		Graphics2D g = bimage.createGraphics();
		AffineTransform gt = new AffineTransform();
		gt.translate(0, image.getHeight(null));
		gt.scale(1.0, -1.0);
		g.drawImage(image, gt, null);
    	
		return ByteBuffer.wrap(((DataBufferByte)raster.getDataBuffer()).getData());
    }
    
    public static void drawPixelsFromBuffer(GL gl, Buffer buffer, int width, int height) {
 	    gl.glRasterPos2i(0, 0);

 		gl.glBlendFunc (GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_ALPHA);
 		gl.glEnable (GL.GL_BLEND);
// 		gl.glColor3f (0.0f, 0.5f, 0.0f);
// 		gl.glRecti (0, 300, 100, 330);
// 		gl.glColor3f (0.0f, 0.0f, 0.0f);
// 		gl.glRasterPos2i (10, 300);
 	    gl.glDrawPixels (width, height,
 	    				 GL.GL_RGBA, GL.GL_UNSIGNED_BYTE,
 	    				 buffer);
 	    
 	    gl.glFlush();
    }
    
    public static void rescale(GL gl, GLU glu, int oldWidth, int oldHeight, int newWidth, int newHeight) {
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glLoadIdentity();
		glu.gluOrtho2D(0, 1, 0, 1);
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadIdentity();
    }

    public static void drawPixelsRescale(GL gl, GLU glu, int oldWidth, int oldHeight, int newWidth, int newHeight) {
    	rescale(gl, glu, oldWidth, oldHeight, newWidth, newHeight);
		// This zoom is quite bad, much aliasing...
		gl.glPixelZoom( (float)newWidth / (float)oldWidth, (float)newHeight / (float)oldHeight);
    }
    
    // XXX: pour une raison obscure ceci ne fonctionne pas...
    public static void drawPixelsFromImage(GL gl, Image image) {
    	Buffer buffer = createPixelDrawingBufferFromImage(image);
    	drawPixelsFromBuffer(gl, buffer, image.getWidth(null), image.getHeight(null));		
    }

    public static TextureData createTextureDataFromText(String text, Font font, int width, int height) {
    	// First create buffered image.
    	BufferedImage img = new BufferedImage(width, height, BufferedImage.TYPE_INT_BGR);
    	Graphics2D gc = img.createGraphics();
    	gc.setRenderingHint (RenderingHints.KEY_ANTIALIASING,
                			 RenderingHints.VALUE_ANTIALIAS_ON);
    	gc.setColor(Color.LIGHT_GRAY);
    	gc.fillRect(0,0,width,height);
    	gc.setFont(font);
    	gc.setColor(Color.BLACK);
    	gc.drawString(text, 10, height*0.66f);

    	return createTextureDataFromImage(img);
    }
    
    public static void drawSquareTexture(GL gl, Texture texture) {
		gl.glClearDepth( 1.0 );
//		gl.glClear(GL.GL_COLOR_BUFFER_BIT /*| GL.GL_DEPTH_BUFFER_BIT*/ ); // clear the screen
		gl.glEnable(GL.GL_DEPTH_TEST);
		gl.glDepthFunc(GL.GL_LEQUAL);
		gl.glEnable(GL.GL_LIGHTING);
		
		// draw
		{
		    texture.enable();
			texture.bind();
		    
			gl.glTexEnvi(GL.GL_TEXTURE_ENV, GL.GL_TEXTURE_ENV_MODE, GL.GL_REPLACE);
			TextureCoords coords = texture.getImageTexCoords();
			
			gl.glBegin(GL.GL_QUADS);
			gl.glTexCoord2f(coords.left(), coords.bottom());
			gl.glVertex3f(0, 0, 0);
			gl.glTexCoord2f(coords.right(), coords.bottom());
			gl.glVertex3f(1, 0, 0);
			gl.glTexCoord2f(coords.right(), coords.top());
			gl.glVertex3f(1, 1, 0);
			gl.glTexCoord2f(coords.left(), coords.top());
			gl.glVertex3f(0, 1, 0);
			gl.glEnd();
			
			texture.disable();
		}
		
		gl.glDisable(GL.GL_LIGHTING);
		gl.glDisable(GL.GL_DEPTH_TEST);				
    }
}
