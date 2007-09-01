package drone.serial.demo;

import drone.serial.lib.Serial;

public class SerialDemo {

	public static void main(String[] args) {
		Serial mySerial = new Serial("/dev/ttyUSB0", 9600);
		mySerial.write('A');
		char read;
		while ((read = mySerial.readChar()) == (char)(-1))
			System.out.print("."); System.out.flush();
		if (read == 'A')
			System.out.println("Yeah!");
		else
			System.out.println("Error : " + read);
		System.exit(0);
	}
}
