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
package drone.jmf.actors;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Image;
import java.util.LinkedList;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import ptolemy.actor.lib.Sink;
import ptolemy.data.ImageToken;
import ptolemy.data.Token;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.InternalErrorException;
import ptolemy.kernel.util.KernelException;
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
        if (input.hasToken(0)) {
            final Token in = input.get(0);

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
                _picture.setImage(((ImageToken) in).asAWTImage());
                _picture.displayImage();
                _picture.repaint();
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
