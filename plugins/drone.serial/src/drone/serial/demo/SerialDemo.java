package drone.serial.demo;

import drone.serial.lib.Serial;

public class SerialDemo {

	public static void main(String[] args) throws Exception {
		String serialName = "/dev/ttyUSB0";
		if (args.length > 0) {
			String[] ports = Serial.serialPortList();
			if (args[0].equals("list")) {
				for (int i=0; i<ports.length; i++)
					System.out.println(ports[i]);
				return;
			} else {
				serialName = ports[Integer.parseInt(args[0])];
			}
		}
		System.out.println("Opening serial port '" + serialName + "'");
		Serial mySerial = Serial.getSerial(serialName);
//		Serial mySerial2 = new Serial("/dev/ttyUSB0", 9600);
		mySerial.write('A');
		char read;
		while ((read = mySerial.readChar()) == (char)(-1))
			System.out.print("."); System.out.flush();
		if (read == 'B')
			System.out.println("Yeah!");
		else
			System.out.println("Error : " + read);
		System.exit(0);
	}
}
