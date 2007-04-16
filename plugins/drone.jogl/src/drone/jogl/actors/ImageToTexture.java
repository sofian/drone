/* Converts an image to a texture.
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
package drone.jogl.actors;

import java.awt.Component;
import java.awt.Image;

import com.sun.opengl.util.texture.TextureData;

import ptolemy.actor.lib.Transformer;
import ptolemy.data.ImageToken;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;


//////////////////////////////////////////////////////////////////////////
//// TextString3D

/**
 TODO: write the documentation for this
 An actor that encapsulates 3D text shapes in the GR domain.
 The <i>text</i> port/parameter gives the text to be displayed.
 The <i>fontSize</i> parameter gives the size of the font.
 The <i>extrusionDepth</i> parameter specifies how deep the 3-D
 rendering of the text should be.
 The <i>alignment</i> parameter gives the alignment of the text
 relative to the position of the object.
 The <i>fontFamily</i> parameter specifies the font family.
 The rest of the parameters are described in the base class.

 @author C. Fong and Edward A. Lee
 @version $Id: TextString3D.java,v 1.39 2006/04/02 07:04:51 cxh Exp $
 @since Ptolemy II 1.0
 @Pt.ProposedRating Green (eal)
 @Pt.AcceptedRating Green (liuxj)
 */
public class ImageToTexture extends Transformer {
    /** Construct an actor with the given container and name.
     *  @param container The container.
     *  @param name The name of this actor.
     *  @exception IllegalActionException If the actor cannot be contained
     *   by the proposed container.
     *  @exception NameDuplicationException If the container already has an
     *   actor with this name.
     */
    public ImageToTexture(CompositeEntity container, String name)
            throws IllegalActionException, NameDuplicationException {
        super(container, name);
        input.setTypeEquals(BaseType.OBJECT);
        output.setTypeEquals(BaseType.OBJECT);
    }

    ///////////////////////////////////////////////////////////////////
    ////                         ports and parameters              ////

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** If the attribute is <i>text</i>, then update the displayed
     *  text string.
     *  @param attribute The attribute.
     *  @exception IllegalActionException If thrown by the parent class.
     */
    public void attributeChanged(Attribute attribute)
            throws IllegalActionException {
            super.attributeChanged(attribute);
    }

    /** Override the base class to update the <i>text</i> parameter
     *  from the value given at the port, if any.
     *  @return False if the scene graph has already been
     *   initialized.
     *  @exception IllegalActionException If thrown by the parent class.
     */
    public boolean prefire() throws IllegalActionException {
    	if (input.hasToken(0)) {
    		Image image = ((ImageToken)input.get(0)).asAWTImage();
    		if (image != null) {
    			_textureData = JOGLUtils.createTextureDataFromImage(image);
    		}
    	}
        return super.prefire();
    }
    
    /** Do nothing.  Derived classes override this method to define their
     *  primary run-time action.
     *
     *  @exception IllegalActionException Not thrown in this base class.
     */
    public void fire() throws IllegalActionException {
    	if (_textureData != null)
    		output.send(0, new TextureToken(_textureData));    		
    }
    
    ///////////////////////////////////////////////////////////////////
    ////                     ports and parameters                  ////
    TextureData _textureData = null;
    Component _viewScreen3dCanvas = null;
   
    Image _textureImage = null;

}
