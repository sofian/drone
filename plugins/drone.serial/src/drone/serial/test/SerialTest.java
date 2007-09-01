package drone.serial.test;

import drone.serial.lib.Serial;
import junit.framework.TestCase;

public class SerialTest extends TestCase {

	Serial serial;
	
	static int DEFAULT_RATE = 9600;
	
	public void setUp() throws Exception {
		serial = new Serial(getPortName(), DEFAULT_RATE);
	}

	public static String getPortName() throws Exception {
		String os = System.getProperty("os.name");
		if (os.equals("Mac OS X"))
			return "/dev/ttyUSB0";
		else if (os.equals("Linux"))
			return "/dev/ttyUSB0";
		else if (os.equals("Windows"))
			return "COM1";
		else
			throw new Exception("Unrecognized OS: " + os);
	}
	
	public void tearDown() {
		serial.stop();
	}
	
	public void testSerialEvent() {
		serial.write(10);
		int read;
		while ((read = serial.read()) == (char)(-1))
		assertEquals(read, 10);
	}

//	public void testBuffer() {
//		fail("Not yet implemented");
//	}
//
//	public void testAvailable() {
//		fail("Not yet implemented");
//	}
//
//	public void testClear() {
//		fail("Not yet implemented");
//	}
//
//	public void testRead() {
//		fail("Not yet implemented");
//	}
//
//	public void testLast() {
//		fail("Not yet implemented");
//	}
//
//	public void testReadChar() {
//		fail("Not yet implemented");
//	}
//
//	public void testReadBytes() {
//		fail("Not yet implemented");
//	}
//
//	public void testReadString() {
//		fail("Not yet implemented");
//	}
//
//	public void testWriteInt() {
//		fail("Not yet implemented");
//	}

}
