package ptolemy.actor.lib.artkp.jogl;

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
