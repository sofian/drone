/* Convert an ImageToken into a JMFImageToken.
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
package drone.jmf.actors;

import java.awt.Image;

import javax.media.Buffer;
import javax.media.util.ImageToBuffer;

import ptolemy.actor.lib.Transformer;
import ptolemy.data.ImageToken;
import ptolemy.data.IntToken;
import ptolemy.data.expr.Parameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

import drone.jmf.data.JMFImageToken;

//////////////////////////////////////////////////////////////////////////
//// ImageToJMF

/**
 Convert an ImageToken into a JMFImageToken.

 @author Christopher Hylands Brooks
 @version $Id: ImageToJMF.java,v 1.10 2005/07/08 19:56:31 cxh Exp $
 @since Ptolemy II 3.1
 @Pt.ProposedRating Red (cxh)
 @Pt.AcceptedRating Red (cxh)
 */
public class ImageToJMF extends Transformer {
    /** Construct an actor with the given container and name.
     *  @param container The container.
     *  @param name The name of this actor.
     *  @exception IllegalActionException If the actor cannot be contained
     *   by the proposed container.
     *  @exception NameDuplicationException If the container already has an
     *   actor with this name.
     */
    public ImageToJMF(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);
        frameRate = new Parameter(this, "frameRate");
        frameRate.setExpression("15");

        input.setTypeEquals(BaseType.OBJECT);
        output.setTypeEquals(BaseType.OBJECT);
    }

    ///////////////////////////////////////////////////////////////////
    ////                     ports and parameters                  ////

    /** The frame rate in frames per second at which the images are
     *  being generated.  If video is being generated, then the frame
     *  rate is usually between 1 and 60.  The initial default is
     *  an integer with value 15, meaning 15 frames per second
     */
    public Parameter frameRate;

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** Override the base class and determine the data type to format
     *  the data to, as well as whether to scale the data.
     *  @param attribute The attribute that changed.
     *  @exception IllegalActionException If the base class throws it,
     *  or if the data type is not recognized.
     */
    public void attributeChanged(Attribute attribute)
            throws IllegalActionException {
        if (attribute == frameRate) {
            _frameRate = ((IntToken) frameRate.getToken()).intValue();
        } else {
            super.attributeChanged(attribute);
        }
    }

    /** Fire this actor.
     *  Output the JAIImageToken constructed from the matrix of doubles.
     *  @exception IllegalActionException If a contained method throws it,
     *   or if a token is received that contains a null image.
     */
    public void fire() throws IllegalActionException {
        super.fire();

        if (input.hasToken(0)) {
            ImageToken imageToken = (ImageToken) input.get(0);
            Image image = imageToken.asAWTImage();
            Buffer buffer = ImageToBuffer.createBuffer(image, _frameRate);
            output.send(0, new JMFImageToken(buffer));
        }
    }

    ///////////////////////////////////////////////////////////////////
    ////                         private variables                 ////
    // The frame rate in frames per second.
    private int _frameRate;
}
