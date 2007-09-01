package drone.serial.demo;

import gnu.io.*;
import java.util.*;

public class CheckSerialPortDemo {

	public CheckSerialPortDemo() {
	}

	public static void main(String args[])
	{

		Enumeration ports = CommPortIdentifier.getPortIdentifiers();
		while(ports.hasMoreElements())
		{
			System.out.println((CommPortIdentifier) ports.nextElement());
		}

	}

}

