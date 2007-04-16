package ptolemy.actor.lib.artkp;

import javax.media.opengl.GL;

public class ARTKPUtils {

	public static double ZOOM = 1.0;
	public static int GMINI = 2;
	public static int MINIWIN_MAX = 8;
	
	public static void argDrawMode3D(GL gl) {
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadIdentity();
	}
	
	public static void argDrawMode2D(GL gl, int widthWin, int heightWin)
	{
		gl.glMatrixMode(GL.GL_MODELVIEW);
	    gl.glLoadIdentity();
	    gl.glMatrixMode(GL.GL_PROJECTION);
	    gl.glLoadIdentity();
	    gl.glOrtho(0, widthWin, 0, heightWin, -1.0, 1.0);
	    gl.glViewport(0, 0, widthWin, heightWin);

	    argSetStencil( gl, 0 );
	}
	
	public static void argDraw3dCamera(GL gl,
									   int xWin, int yWin, 
									   int widthWin, int heightWin,
									   int widthCam, int heightCam,
									   float[] projectionMatrix)
	{
		int widthMini = (int) ((double)widthCam * ZOOM / (double)GMINI);
		int heightMini = (int) ((double)heightCam * ZOOM / (double)GMINI);
		
		// This code was adapted from ARToolkit's argDrawCamera3d() function in file gsub.c.
	    if( xWin == 0 && yWin == 0 ) {
	        gl.glViewport(0, heightWin - (int) (ZOOM * heightCam),
	        			  (int) (ZOOM * widthCam), (int) (ZOOM * heightCam));
	    }
	    else {
	        gl.glViewport((xWin-1)*widthMini, heightWin-heightCam-yWin*heightMini,
	        			  widthMini, heightMini);
	    }

	    argSetStencil( gl, 0 );
	    
	    gl.glMatrixMode(GL.GL_PROJECTION);
	    gl.glLoadMatrixf( projectionMatrix, 0 );
	}

	static void argSetStencil(GL gl, int flag )
	{
	    if( flag == 0 ) {
	        gl.glDisable(GL.GL_STENCIL_TEST);
	        gl.glStencilFunc (GL.GL_ALWAYS, 0, 0);
	        gl.glStencilOp (GL.GL_KEEP, GL.GL_KEEP, GL.GL_KEEP);
	    }
	    else {
	    	gl.glEnable(GL.GL_STENCIL_TEST);
	    	gl.glStencilFunc (GL.GL_EQUAL, flag, flag);
	    	gl.glStencilOp (GL.GL_KEEP, GL.GL_KEEP, GL.GL_KEEP);
	    }
	}

}
