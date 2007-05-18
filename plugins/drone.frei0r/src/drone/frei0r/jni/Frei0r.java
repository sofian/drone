/* A Java bridge to the Frei0r video effect API.
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
package drone.frei0r.jni;

import java.awt.Point;
import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.awt.image.DataBufferInt;
import java.awt.image.Raster;
import java.net.URL;

import drone.frei0r.Frei0rException;
import drone.util.ImageConvert;

/**
 * Provide a bridge for Frei0r, minimalistic plugin API for video effects.
 * http://www.piksel.org/frei0r/1.0/spec/index.html
 * 
 * @author Mathieu Guindon, Jean-Sebastien Senecal
 */
public class Frei0r {
	
	/**
	 * The frei0r API major version
	 */
	public static final int FREI0R_MAJOR_VERSION  = 1;

	/**
	 * The frei0r API minor version
	 */
	public static final int FREI0R_MINOR_VERSION = 1;

	/** one input and one output */
	public static final int F0R_PLUGIN_TYPE_FILTER = 0;
	/** just one output */
	public static final int F0R_PLUGIN_TYPE_SOURCE = 1;
	/** two inputs and one output */
	public static final int F0R_PLUGIN_TYPE_MIXER2 = 2;
	/** three inputs and one output */
	public static final int F0R_PLUGIN_TYPE_MIXER3 = 3;

	/**
	 * In BGRA8888, each pixel is represented by 4 consecutive
	 * unsigned bytes, where the first byte value represents
	 * the blue, the second the green, and the third the red color
	 * component of the pixel. The last value represents the
	 * alpha value.
	 */
	public static final int F0R_COLOR_MODEL_BGRA8888 = 0;

	/**
	 * In RGBA8888, each pixel is represented by 4 consecutive
	 * unsigned bytes, where the first byte value represents
	 * the red, the second the green, and the third the blue color
	 * component of the pixel. The last value represents the
	 * alpha value.
	 */
	public static final int F0R_COLOR_MODEL_RGBA8888 = 1;

	/** \addtogroup PARAM_TYPE Parameter Types
	 *
	 *  @{
	 */


	/**
	 * Parameter type for boolean values
	 * \see f0r_param_bool
	 */
	public static final int F0R_PARAM_BOOL = 0;

	/**
	 * Parameter type for doubles
	 * \see f0r_param_double
	 */
	public static final int F0R_PARAM_DOUBLE = 1;

	/**
	 * Parameter type for color
	 * \see f0r_param_color
	 */
	public static final int F0R_PARAM_COLOR = 2;
	
	/**
	 * Parameter type for position
	 * \see f0r_param_position
	 */
	public static final int F0R_PARAM_POSITION = 3;

	/**
	 * Parameter type for string
	 * \see f0r_param_string
	 */
	public static final int F0R_PARAM_STRING = 4;

	/**
	 * In PACKED32, each pixel is represented by 4 consecutive
	 * bytes, but it is not defined how the color componets are
	 * stored. The true color format could be RGBA8888,
	 * BGRA8888, a packed 32 bit YUV format, or any other
	 * color format that stores pixels in 32 bit.
	 *
	 * This is useful for effects that don't work on color but
	 * only on pixels (for example a mirror effect).
	 *
	 * Note that source effects must not use this color model.
	 */
	public static final int F0R_COLOR_MODEL_PACKED32 = 2;


	public Frei0r(String frei0rLibName) throws Frei0rException {
		try {
			// Try to open it as is.
			_openLibrary(frei0rLibName);
		} catch (Frei0rException e) {
			// Try to get it as a resource.
			ClassLoader cl = this.getClass().getClassLoader();
			URL path = cl.getResource(frei0rLibName);
			if (path == null) {
				throw new Frei0rException("Cannot open given frei0r library " + frei0rLibName + 
											": cannot find path to resource and/or failed to open because: " + e.getMessage());
			}
			_openLibrary(path.getFile());
		}
		if (_handle == 0)
			throw new Frei0rException("Handle not set.");
	}

	protected void finalize() throws Frei0rException {
		deinit();
		if (_handle != 0)
			_closeLibrary();
	}

	public Instance createInstance(int width, int height) throws Frei0rException {
		return new Instance(this, width, height);
	}

	// Public methods.
	/**< The (short) name of the plugin                   */
	public native String getName() throws Frei0rException;
	
	/**< The plugin author                                */
	public native String getAuthor() throws Frei0rException;
	/** The plugin type
	 * \see PLUGIN_TYPE
	 */
	
	public native int getPluginType() throws Frei0rException;
	
    /**< The color model used                             */
	public native int getColorModel() throws Frei0rException;
	
	/**< The frei0r major version this plugin is built for*/
	public native int getFrei0rVersion() throws Frei0rException;
	
	 /**< The major version of the plugin                  */
	public native int getMajorVersion() throws Frei0rException;
	
	  /**< The minor version of the plugin                  */
	public native int getMinorVersion() throws Frei0rException;
	
    /**< The number of parameters of the plugin           */
	public native int nParams() throws Frei0rException;
	
	/**< An optional explanation string               */
	public native String getExplanation() throws Frei0rException;

	// /////////////////////////////////////////////////////////////////
	// // JNI section ////

	static {
		System.loadLibrary("Frei0rJNI");
	}

	// Protected methods.
	
	/**
	 * Creates a handle to a frei0r dynamic library.
	 * @param libName the full path to the dynamic library
	 * @return a handle to the library
	 */
	protected native void _openLibrary(String libName) throws Frei0rException;

	protected native void _closeLibrary();

	// TODO: these methods should be protected
	public native int init() throws Frei0rException;

	public native void deinit() throws Frei0rException;

	public native long construct(int width, int height) throws Frei0rException;

	public native void destruct(long instance) throws Frei0rException;

	public native void update(long instance, double time, int[] inframe, int[] outframe) throws Frei0rException;

	public native void update2(long instance, double time,
			int[] inframe1, int[] inframe2, int[] inframe3,
			int[] outframe) throws Frei0rException;
	
	public native String getParamName(int paramIndex) throws Frei0rException;
	
	public native int getParamType(int paramIndex) throws Frei0rException;
	
	public native String getParamExplanation(int paramIndex) throws Frei0rException;
	
	public native void setParamValue(long instance, Object param, int paramIndex) throws Frei0rException;
	
	public native Object getParamValue(long instance, int paramIndex) throws Frei0rException;
	
	// XXX: there should be different instance types for different plugins (eg. source, mixer2, mixer3)
	public static class Instance {

		protected Instance(Frei0r parent, int width, int height) throws Frei0rException {
			if (width < 0 || height < 0)
				throw new Frei0rException("Wrong dimensions (negative value): " + width + "x" + height);
			_width = width;
			_height = height;
			_instanceHandle = parent.construct(width, height);
			_parent = parent;
			_size = _width * _height;
		}

		protected void finalize() throws Frei0rException {
			if (_instanceHandle != 0)
				_parent.destruct(_instanceHandle);
		}

		public int getWidth() { return _width; }

		public int getHeight() { return _height; }
		
		public int getSize() { return _size; }
		
		public Frei0r getParent() { return _parent; }

		public void update(double time, BufferedImage in, BufferedImage out) throws Frei0rException {
			int[] inframe = null;
			
			// Sanity check and extract data buffers.
			if (_parent.getPluginType() != Frei0r.F0R_PLUGIN_TYPE_SOURCE) {
				inframe = _inputImageToFrame(in);
			}
			_verifyImage(out);
			
			// Output frame buffer.
			int[] outframe = new int[_size];

			// Update frames.
			update(time, inframe, outframe);
			
			// Set output frame.
			_outputImageSetFrame(out, outframe);
		}

		public void update(double time, int[] inframe, int[] outframe) throws Frei0rException {
			// Sanity check.
			if (_parent.getPluginType() != Frei0r.F0R_PLUGIN_TYPE_SOURCE) {
				_verifyFrame(inframe);
			}
			_verifyFrame(outframe);
			
			// Update.
			_parent.update(_instanceHandle, time, inframe, outframe);
		}

		public void update2(double time, BufferedImage in1, BufferedImage in2,
								BufferedImage in3, BufferedImage out) throws Frei0rException {
			int[] inframe1 = null;
			int[] inframe2 = null;
			int[] inframe3 = null;
			
			// Sanity check and extract data buffers.
			switch (_parent.getPluginType()) {
			case Frei0r.F0R_PLUGIN_TYPE_MIXER3:
				inframe3 = _inputImageToFrame(in3);
			case Frei0r.F0R_PLUGIN_TYPE_MIXER2:
				inframe2 = _inputImageToFrame(in2);
			case Frei0r.F0R_PLUGIN_TYPE_FILTER:
				inframe1 = _inputImageToFrame(in1);
			}
			_verifyImage(out);

			// Output frame buffer.
			int[] outframe = new int[_size];

			// Update frames.
			update2(time, inframe1, inframe2, inframe3, outframe);

			// Set output frame.
			_outputImageSetFrame(out, outframe);
		}

		public void update2(double time, int[] inframe1, int[] inframe2, int[] inframe3, int[] outframe) throws Frei0rException {
			// Sanity check.
			switch (_parent.getPluginType()) {
			case Frei0r.F0R_PLUGIN_TYPE_MIXER3:
				_verifyFrame(inframe3);
			case Frei0r.F0R_PLUGIN_TYPE_MIXER2:
				_verifyFrame(inframe2);
			case Frei0r.F0R_PLUGIN_TYPE_FILTER:
				_verifyFrame(inframe1);
			}
			_verifyFrame(outframe);

			// Update.
			_parent.update2(_instanceHandle, time, inframe1, inframe2, inframe3, outframe);
		}
		
		private void _verifyFrame(int[] frame) throws Frei0rException {
			// Check for null.
			if (frame == null)
				throw new NullPointerException("Frame is null.");
			// Check dimensions.
			if (frame.length != _size)
				throw new Frei0rException("Frame has wrong size " + frame.length + ", should be " + _size);
		}
		
		private void _verifyImage(BufferedImage img) throws Frei0rException {
			// Check for null.
			if (img == null)
				throw new NullPointerException("Input image is null.");
			// Check dimensions.
			if (_width != img.getWidth() || _height != img.getHeight())
				throw new Frei0rException("Input image has wrong dimensions: " +
						img.getWidth() + "x" + img.getHeight() + ", should be " +
						_width + "x" + _height);
			// Make sure images are of TYPE_INT_ARGB.
			if (img.getType() != BufferedImage.TYPE_INT_ARGB) {
				throw new Frei0rException("Output image must be of type ARGB.");
			}
		}
		
		private int[] _inputImageToFrame(BufferedImage img) throws Frei0rException {
			_verifyImage(img);
			// Make sure the image has the right format.
			img = ImageConvert.convertType(img, BufferedImage.TYPE_INT_ARGB);
			// Get and verify data buffer.
			DataBuffer buffer = img.getData().getDataBuffer();
			if (buffer.getDataType() != DataBuffer.TYPE_INT) {
				throw new Frei0rException("Data buffer of input image has wrong type.");
			}
			// Convert input to ABGR if needed.
			int[] data = ((DataBufferInt)buffer).getData();
			if (_parent.getColorModel() == F0R_COLOR_MODEL_RGBA8888) {
				ImageConvert.convertARGBtoABGR(data);
			}
			return data;
		}
		
		private void _outputImageSetFrame(BufferedImage img, int[] frame) throws Frei0rException {
			// Convert output to ABGR if needed.
			if (_parent.getColorModel() == F0R_COLOR_MODEL_RGBA8888) {
				ImageConvert.convertABGRtoARGB(frame);
			}
			// Copy data to the output image buffer.
			// XXX it would be nice to find a way NOT to copy the data
			img.setRGB(0, 0, _width, _height, frame, 0, _width);
		}
		
		public Object getParamValue(int paramIndex) throws Frei0rException {
			return _parent.getParamValue(_instanceHandle, paramIndex);
		}
		
		public void setParamValue(Object param, int paramIndex) throws Frei0rException {
			_parent.setParamValue(_instanceHandle, param, paramIndex);
		}

		private long _instanceHandle;
		private Frei0r _parent;
		private int _width, _height, _size;
	}

	public static class Color {
		public Color(float r, float g, float b) {
			_r = r;
			_g = g;
			_b = b;
		}
		public float getRed() { return _r; }
		public float getGreen() { return _g; }
		public float getBlue() { return _b; }

		public void setRed(float r) { _r = r; }
		public void setGreen(float g) { _g = g; }
		public void setBlue(float b) { _b = b; }
		
		private float _r;
		private float _g;
		private float _b;
	}

	public static class Position {
		public Position(double x, double y) {
			_x = x;
			_y = y;
		}
		public double getX() { return _x; }
		public double getY() { return _y; }
		
		public void setX(double x) { _x = x; }
		public void setY(double y) { _y = y; }
		
		private double _x;
		private double _y;
	}
		
	// /////////////////////////////////////////////////////////////////
	// // private variables ////

	/**
	 * Used by native code exclusively. Handle to the dynamic library.
	 */
	@SuppressWarnings("unused")
	private long _handle;

}
