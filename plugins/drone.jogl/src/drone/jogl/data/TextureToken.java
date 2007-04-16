/* A token containing a JOGL texture.
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
 package drone.jogl.data;

import java.awt.image.BufferedImage;

import com.sun.opengl.util.texture.Texture;
import com.sun.opengl.util.texture.TextureData;
import com.sun.opengl.util.texture.TextureIO;

import ptolemy.data.Token;
import ptolemy.data.type.BaseType;
import ptolemy.data.type.Type;

//////////////////////////////////////////////////////////////////////////
////ImageToken

/**
An abstract class that all tokens which encapsulate images should extend.
Each class that extends this class is required to implement the method
asAWTImage(), so that it may be used with the standard image processing
library.

@author James Yeh
@version $Id: ImageToken.java,v 1.19 2006/03/28 23:59:10 cxh Exp $
@since Ptolemy II 3.0
@Pt.ProposedRating Red (cxh)
@Pt.AcceptedRating Red (cxh)
*/
public class TextureToken extends Token {

	public static final BufferedImage DUMMY_IMAGE = new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB_PRE);

	
	/** Construct default token.
     */
	public TextureToken() {
		super();
		_textureData = TextureIO.newTextureData(DUMMY_IMAGE, false);
	}
	
	/** Construct a token with a basic TextureLoader inside.
     */
	public TextureToken(TextureData textureData) {
		super();
		_textureData = textureData;
	}
	
    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** Return the type of this token.
     *  @return BaseType.OBJECT
     */
    public Type getType() {
        return BaseType.OBJECT;
    }
	
    public TextureData getTextureData() {
    	return _textureData;
    }
    	
	protected TextureData _textureData = null;

}
