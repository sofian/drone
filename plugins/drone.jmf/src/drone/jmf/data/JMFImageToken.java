/* A token that contains a javax.media.Buffer.
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

package drone.jmf.data;

import java.awt.Image;

import javax.media.Buffer;
import javax.media.format.VideoFormat;
import javax.media.util.BufferToImage;

import ptolemy.data.ImageToken;
import ptolemy.data.type.BaseType;
import ptolemy.data.type.Type;

//////////////////////////////////////////////////////////////////////////
//// JMFImageToken

/**
 A token that contains a javax.media.Buffer.  This token is used when
 dealing with images in the Java Media Framework (JMF) library.  Because
 it extends ImageToken, it can be used with the standard image processing
 tools by simply casting it, and calling asAWTImage().

 @author James Yeh
 @version $Id: JMFImageToken.java,v 1.26 2006/08/21 23:11:46 cxh Exp $
 @since Ptolemy II 3.0
 @Pt.ProposedRating Red (cxh)
 @Pt.AcceptedRating Red (cxh)
 */
public class JMFImageToken extends ImageToken {
    /** Construct a token with a specified Buffer.
     *  @param value The buffer.
     */
    public JMFImageToken(Buffer value) {
        _value = value;
    }

    ///////////////////////////////////////////////////////////////////
    ////                         public methods                    ////

    /** Convert a javax.media.Buffer to a java.awt.Image and return it.
     *  A new javax.media.util.BufferToImage is created when the
     *  video format of the Buffer changes.
     */
    public Image asAWTImage() {
    	if (_value == null)
    		return null;
    	
        if ((_bufferToImage == null) || (_videoFormat != _value.getFormat())) {
            _videoFormat = (VideoFormat) _value.getFormat();
            if (_videoFormat == null)
            	return null;
            
            _bufferToImage = new BufferToImage(_videoFormat);
        }

        _awtImage = _bufferToImage.createImage(_value);
        return _awtImage;
    }

    /** Return the type of this token.
     *  @return BaseType.OBJECT
     */
    public Type getType() {
        return BaseType.OBJECT;
    }

    /** Return the value of this token.
     *  @return The value of this token.   
     */
    public Buffer getValue() {
        return _value;
    }

    /** Return a description of the token.
     *  If possible, derived classes should override this method and
     *  return the value of this token as a string that can be parsed
     *  by the expression language to recover a token with the same value.
     *  Unfortunately, in this base class, we can only return the
     *  a partial description of the token.
     *  @return A string representation of a record that partially
     *  describes the token.
     */
    public String toString() {
        // FIXME: return a value that can be parsed by the expression language.
        Image image = asAWTImage();
        return "{type=\"" + getClass() + "\" width=\"" + image.getWidth(null)
                + "\" height=\"" + image.getHeight(null) + "\" format=\""
                + _videoFormat + "\"}";
    }

    ///////////////////////////////////////////////////////////////////
    ////                         private variables                 ////
    private Buffer _value;

    private static BufferToImage _bufferToImage;

    private VideoFormat _videoFormat;

    private Image _awtImage;
}
