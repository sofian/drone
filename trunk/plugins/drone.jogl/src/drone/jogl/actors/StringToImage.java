/* Converts a string into an image.
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

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.GraphicsEnvironment;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;

import ptolemy.actor.lib.Transformer;
import ptolemy.data.AWTImageToken;
import ptolemy.data.DoubleToken;
import ptolemy.data.ImageToken;
import ptolemy.data.IntToken;
import ptolemy.data.StringToken;
import ptolemy.data.Token;
import ptolemy.data.expr.Parameter;
import ptolemy.data.expr.StringParameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

public class StringToImage extends Transformer {
	
	public static final String STYLE_PLAIN = "Plain";
	public static final String STYLE_BOLD = "Bold";
	public static final String STYLE_ITALIC = "Italic";
	public static final String STYLE_BOLD_ITALIC = "Bold italic";
	
	/** Construct an actor with the given container and name.
	 *  @param container The container.
	 *  @param name The name of this actor.
	 *  @exception IllegalActionException If the actor cannot be contained
	 *   by the proposed container.
	 *  @exception NameDuplicationException If the container already has an
	 *   actor with this name.
	 */
	public StringToImage(CompositeEntity container, String name)
	throws IllegalActionException, NameDuplicationException {
		super(container, name);
		input.setTypeEquals(BaseType.STRING);
		output.setTypeEquals(BaseType.OBJECT);
		
		_font = new Font(null, Font.PLAIN, 12);

		fontFamily = new StringParameter(this, "fontFamily");
		fontFamily.setTypeEquals(BaseType.STRING);
		String[] familyNames = GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames();
		fontFamily.setExpression(familyNames[0]);
		for (int i=0; i<familyNames.length; i++)
			fontFamily.addChoice(familyNames[i]);
		
		fontStyle = new StringParameter(this, "fontStyle");
		fontStyle.setTypeEquals(BaseType.STRING);
		fontStyle.setExpression(STYLE_PLAIN);
		fontStyle.addChoice(STYLE_PLAIN);
		fontStyle.addChoice(STYLE_BOLD);
		fontStyle.addChoice(STYLE_ITALIC);
		fontStyle.addChoice(STYLE_BOLD_ITALIC);
		
		fontSize = new Parameter(this, "fontSize");
		fontSize.setTypeEquals(BaseType.DOUBLE);
		fontSize.setExpression("12");
		
		imageWidth = new Parameter(this, "imageWidth");
		imageWidth.setTypeEquals(BaseType.INT);
		imageWidth.setExpression("320");
		
		imageHeight = new Parameter(this, "imageHeight");
		imageHeight.setTypeEquals(BaseType.INT);
		imageHeight.setExpression("240");
		
		textX = new Parameter(this, "textX");
		textX.setTypeEquals(BaseType.INT);
		textX.setExpression("10");
		
		textY = new Parameter(this, "textY");
		textY.setTypeEquals(BaseType.INT);
		textY.setExpression("120");
//		useTrueType = new Parameter(this, "useTrueType");
//		useTrueType.setTypeEquals(BaseType.BOOLEAN);
//		useTrueType.setExpression("true");
	}
	
	///////////////////////////////////////////////////////////////////
	////                         public methods                    ////
	public void attributeChanged(Attribute attribute)
	throws IllegalActionException {
		if (attribute == fontFamily) {
			String familyFontName = ((StringToken)fontFamily.getToken()).stringValue();
			if (!_font.getFamily().equals(familyFontName))
				_font = new Font(familyFontName, _font.getStyle(), _font.getSize());
		} else if (attribute == fontStyle) {
			String fontStyleName = ((StringToken)fontStyle.getToken()).stringValue();
			int fontStyleValue = _font.getStyle();
			if (fontStyleName.equals(STYLE_PLAIN)) {
				fontStyleValue = Font.PLAIN;
			} else if (fontStyleName.equals(STYLE_BOLD)) {
				fontStyleValue = Font.BOLD;
			} else if (fontStyleName.equals(STYLE_ITALIC)) {
				fontStyleValue = Font.ITALIC;
			} else if (fontStyleName.equals(STYLE_BOLD_ITALIC)) {
				fontStyleValue = Font.BOLD | Font.ITALIC;
			}
			_font = _font.deriveFont(fontStyleValue);
		} else if (attribute == fontSize) {
			float fontSizeValue = (float)((DoubleToken)fontSize.getToken()).doubleValue();
			_font = _font.deriveFont(fontSizeValue);
		} else if (attribute == imageWidth) {
			_imageWidth = ((IntToken)imageWidth.getToken()).intValue();
		} else if (attribute == imageHeight) {
			_imageHeight = ((IntToken)imageHeight.getToken()).intValue();
		} else if (attribute == textX) {
			_textX = ((IntToken)textX.getToken()).intValue();
		} else if (attribute == textY) {
			_textY = ((IntToken)textY.getToken()).intValue();
		}
		
		super.attributeChanged(attribute);
	}
	/** Read one java.awt.Image from each channel and write
	 *  information about each image to the output port as a
	 *  StringToken.
	 *
	 *  @exception IllegalActionException If there is no director.
	 */
	public void fire() throws IllegalActionException {
		super.fire();
		int width = input.getWidth();
		
		for (int i = 0; i < width; i++) {
			if (input.hasToken(i)) {
				StringToken stringToken = (StringToken) input.get(i);
				
		    	// First create buffered image.
		    	BufferedImage img = new BufferedImage(_imageWidth, _imageHeight, BufferedImage.TYPE_INT_BGR);
		    	Graphics2D gc = img.createGraphics();
		    	gc.setRenderingHint (RenderingHints.KEY_ANTIALIASING,
		                			 RenderingHints.VALUE_ANTIALIAS_ON);
		    	gc.setColor(Color.WHITE);
		    	gc.fillRect(0,0,_imageWidth, _imageHeight);
		    	gc.setColor(Color.BLACK);
		    	gc.setFont(_font);

		        // Draw the string to the BufferedImage
		    	gc.drawString(stringToken.stringValue(), _textX, _textY);
		        
//		        // Draw the BufferedImage to the screen
//		        gc.drawImage(img, 0, 0, this);
//
	
				Token out = new AWTImageToken(img);
				output.broadcast(out);
			}
		}
	}
	
//	public Parameter fontFace;
	
	public StringParameter fontFamily;
	
	public StringParameter fontStyle;
	
	public Parameter fontSize;
	
	public Parameter imageWidth;
	
	public Parameter imageHeight;

	public Parameter textX;
	
	public Parameter textY;
	
	private int _imageWidth;
	private int _imageHeight;
	
	private int _textX;
	private int _textY;
	
	
	protected Font _font;
}
