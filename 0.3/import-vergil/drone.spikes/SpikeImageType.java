import java.awt.*;
import java.awt.image.*;

public class SpikeImageType {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		BufferedImage img = new BufferedImage(10, 10, BufferedImage.TYPE_INT_ARGB_PRE);
		System.out.println(((DataBufferInt)img.getRaster().getDataBuffer()).getData()[0]);
		img.setRGB(0, 0, 1);
		System.out.println(((DataBufferInt)img.getRaster().getDataBuffer()).getData()[0]);
		
		int[] dataArray = new int[10];
		DataBufferInt myBuffer = new DataBufferInt(dataArray, 10);
		System.out.println(myBuffer.getElem(0));
		dataArray[0] = 100;
		System.out.println(myBuffer.getElem(0));
		myBuffer.getData()[0] = 1000;
		System.out.println(myBuffer.getElem(0));
	}

}
