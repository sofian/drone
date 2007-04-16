/* Static class containing the constants used by ARToolKitPlus.
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
package drone.artkp.jni;

public class ARToolKitPlus {
	
	// PIXEL_FORMAT
	public static final int PIXEL_FORMAT_ABGR = 1;
	public static final int PIXEL_FORMAT_BGRA = 2;
	public static final int PIXEL_FORMAT_BGR = 3;
	public static final int PIXEL_FORMAT_RGBA = 4;
	public static final int PIXEL_FORMAT_RGB = 5;
	public static final int PIXEL_FORMAT_RGB565 = 6;
	public static final int PIXEL_FORMAT_LUM = 7;
	
	// UNDIST_MODE
	public static final int UNDIST_NONE = 0;
	public static final int UNDIST_STD = 1;
	public static final int UNDIST_LUT = 2;


	// IMAGE_PROC_MODE
	public static final int IMAGE_HALF_RES = 0;
	public static final int IMAGE_FULL_RES = 1;

	// MARKER_MODE
	public static final int MARKER_TEMPLATE = 0;
	public static final int MARKER_ID_SIMPLE = 1;
	public static final int MARKER_ID_BCH = 2;

	// POSE_ESTIMATOR
	public static final int POSE_ESTIMATOR_ORIGINAL = 0;			// original "normal" pose estimator
	public static final int POSE_ESTIMATOR_ORIGINAL_CONT = 1;		// original "cont" pose estimator
	public static final int POSE_ESTIMATOR_RPP = 2;					// new "Robust Planar Pose" estimator

}
