package drone.frei0r.demo;

import java.awt.image.BufferedImage;

import drone.frei0r.Frei0rException;
import drone.frei0r.jni.Frei0r;
import drone.util.General;

public class Frei0rBench {

	public static final int SIZE = 320*240;
	public static final int N = 30*10;
	
	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		int side = (int) Math.sqrt(SIZE);
		
		Frei0r frei0r = new Frei0r("/usr/local/lib/frei0r-1/sobel." +  General.getNativeLibraryExtension());
		Frei0r.Instance inst = frei0r.createInstance(side, side);
		
		BufferedImage imgIn = new BufferedImage(side, side, BufferedImage.TYPE_INT_ARGB);
		BufferedImage imgOut = new BufferedImage(side, side, BufferedImage.TYPE_INT_ARGB);
		
		for (int i=0; i<N; i++)
			inst.update(0.0, imgIn, imgOut);
	}

}
