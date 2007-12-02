/* Display a java.awt.Image
 *
 * Copyright (C) 2006 Jean-Sebastien Senecal (js@drone.ws)
 * Copyright (c) 2003-2006 The Regents of the University of California.
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
package drone.jmf.actor;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.GraphicsEnvironment;
import java.awt.Image;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import ptolemy.actor.lib.Sink;
import ptolemy.data.BooleanToken;
import ptolemy.data.ImageToken;
import ptolemy.data.Token;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.InternalErrorException;
import ptolemy.kernel.util.NameDuplicationException;
import ptolemy.kernel.util.Workspace;
import ptolemy.media.Picture;


//////////////////////////////////////////////////////////////////////////
//// ImageDisplay

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
public class ImageDisplay extends Sink {
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

        // FIXME: This is required to be an ImageToken, but
        // we don't see to have that class.
        input.setTypeEquals(BaseType.OBJECT);
        
		fullscreen = new Parameter(this, "fullscreen");
		fullscreen.setTypeEquals(BaseType.BOOLEAN);
		fullscreen.setExpression("false");
		
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
        ImageDisplay newObject = (ImageDisplay) super.clone(workspace);

        newObject._frame = null;
        return newObject;
    }
    
	public Parameter fullscreen;

//    /** Get the background.
//     *  @return The background color.
//     *  @see #setBackground(Color)
//     */
//    public Color getBackground() {
//        return _container.getBackground();
//    }
//
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
//        // Place the pane in supplied container.
//        _container.add(_picture, BorderLayout.CENTER);
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

            SwingUtilities.invokeLater(doDisplay);
        }

        return super.postfire();
    }

    /** Set the background.
     *  @param background The background color.
     *  @see #getBackground()
     */
//    public void setBackground(Color background) {
//        _container.setBackground(background);
//    }

    /** Display the specified token. This must be called in the Swing
     *  event thread.
     *  @param in The token to display
     */
    protected void _display(Token in) {
        if (!(in instanceof ImageToken)) {
            throw new InternalErrorException(
                    "Input is not an ImageToken. It is: " + in);
        }

        if (_picture != null) {
            Image image = ((ImageToken) in).asAWTImage();
            
            if (image != null) {
            
	            int xSize = image.getWidth(null);
	            int ySize = image.getHeight(null);
	
	            // If the size has changed, have to recreate the Picture object.
	            if ((_oldXSize != xSize) || (_oldYSize != ySize)) {
	                if (_debugging) {
	                    _debug("Image size has changed.");
	                }
	
	                _oldXSize = xSize;
	                _oldYSize = ySize;
	                
	                Container container = _picture.getParent();
	        
	                if (_picture != null) {
	                    container.remove(_picture);
	                }
	
	                _picture = new Picture(xSize, ySize);
	                _picture.setImage(image);
	                _picture.setBackground(null);
	                _frame.pack();
	                container.add(_picture, BorderLayout.CENTER);
	                container.validate();
	                container.invalidate();
	                container.repaint();
	                container.doLayout();
	
	                Container c = container.getParent();
	
	                while (c.getParent() != null) {
	                    c.invalidate();
	                    c.validate();
	                    c = c.getParent();
	
	                    if (c instanceof JFrame) {
	                        ((JFrame) c).pack();
	                    }
	                }
	            } else {
	                _picture.setImage(image);
	                _picture.displayImage();
	                _picture.repaint();
	            }
            }
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
                    // Regrettably, since setSize() in swing doesn't actually
                    // set the size of the frame, we have to also set the
                    // size of the internal component.
//                    Component[] components = _frame.getContentPane()
//                        .getComponents();
    				
    				if (((BooleanToken)fullscreen.getToken()).booleanValue()) {
    					_frame.setVisible(false);
    					_frame.setUndecorated(true);
    					GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice().setFullScreenWindow(_frame);
    				} else {
    					_frame.setVisible(true);
    				}
                } catch (Exception ex) {
                    throw new InternalErrorException(ex);
                }
        } else {
                if (_frame != null) {
                    // Do not use show() as it overrides manual placement.
                    _frame.toFront();
                }
            }


        	if (_picture == null) {
        		// Create the pane.
        		_picture = new Picture(_oldXSize, _oldYSize);
        		_frame.setSize(_oldXSize, _oldYSize);
        	}

        	_frame.add(_picture);
        	
        	if (_frame != null) {
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

    /** The horizontal size of the previous image. */
    protected int _oldXSize = 0;

    /** The vertical size of the previous image. */
    protected int _oldYSize = 0;

    /** The picture panel. */
    protected Picture _picture;

    ///////////////////////////////////////////////////////////////////
    ////                         private variables                 ////

}
