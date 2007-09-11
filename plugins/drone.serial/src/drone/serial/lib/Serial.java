/* Class for serial port goodness
 * 
 * Adapted from processing.serial.Serial code from 
 * the Processing project http://processing.org
 * Copyright (c) 2004-05 Ben Fry & Casey Reas
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
package drone.serial.lib;

import gnu.io.*;

import java.io.*;
import java.util.*;
import java.lang.reflect.*;

import ptolemy.kernel.Port;
import sun.reflect.generics.tree.Tree;

public class Serial implements SerialPortEventListener {

	// properties can be passed in for default values
	// otherwise defaults to 9600 N81

	// these could be made static, which might be a solution
	// for the classloading problem.. because if code ran again,
	// the static class would have an object that could be closed
	
	public static final int BUFFER_SIZE = 32768;

	private SerialPort _port;

	private String _portName;

	// read buffer and streams
	private InputStream _input;
	private OutputStream _output;

	private byte _buffer[] = new byte[BUFFER_SIZE];
	private int _bufferIndex;
	private int _bufferLast;

	// defaults

	public static final String DEFAULT_PORT_NAME = "COM1";
	public static final int DEFAULT_RATE = 9600;
	public static final int DEFAULT_PARITY = SerialPort.PARITY_NONE;
	public static final int DEFAULT_DATABITS = SerialPort.DATABITS_8;
	public static final int DEFAULT_STOPBITS = SerialPort.STOPBITS_1;

	private static Map<String, Serial> _serials = new TreeMap<String, Serial>();
	
	protected Serial(String portName) throws Exception {
		if (_serials.containsKey(portName))
			throw new Exception("Serial port " + portName + " has already been set");
		_portName = portName;
		init();
	}
	
	public static Serial getSerial(String portName) throws Exception {
		if (_serials.containsKey(portName))
			return _serials.get(portName);
		else {
			Serial serial = new Serial(portName);
			_serials.put(portName, serial);
			return serial;
		}
	}

	public void init() throws Exception {
		CommPortIdentifier portId = CommPortIdentifier.getPortIdentifier(_portName);
		if (portId == null)
			throw new NoSuchPortException();

		if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
			_port = (SerialPort)portId.open("dummy", 2000);
			_input = _port.getInputStream();
			_output = _port.getOutputStream();
			_port.setSerialPortParams(DEFAULT_RATE, DEFAULT_DATABITS, DEFAULT_STOPBITS, DEFAULT_PARITY);
			_port.addEventListener(this);
			_port.notifyOnDataAvailable(true);
		} else {
			throw new Exception("Specified port exists but is not a serial port.");
		}
	}

	/**
	 * Used by PApplet to shut things down.
	 * @throws IOException 
	 */
	public void dispose() throws IOException {
		// do io streams need to be closed first?
		if (_input != null) _input.close();
		if (_output != null) _output.close();
		_input = null;
		_output = null;

		if (_port != null) _port.close();  // close the port
		_port = null;
	}

	public void setSerialPortParams(int baudrate,
									int dataBits,
									int stopBits,
									int parity) throws UnsupportedCommOperationException {
		if (_port != null)
			_port.setSerialPortParams(baudrate, dataBits, stopBits, parity);
	}
	
	/**
	 * Set the DTR line. Addition from Tom Hulbert.
	 * @throws Exception 
	 */
	public void setDTR(boolean state) throws Exception {
		if (_port == null)
			init();
		_port.setDTR(state);
	}

	synchronized public void serialEvent(SerialPortEvent serialEvent) {
		if (serialEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
			try {
				while (_input.available() > 0) {
					synchronized (_buffer) {
						if (_bufferLast == _buffer.length) {
							byte temp[] = new byte[_bufferLast << 1];
							System.arraycopy(_buffer, 0, temp, 0, _bufferLast);
							_buffer = temp;
						}
						_buffer[_bufferLast++] = (byte) _input.read();
					}
				}

			} catch (Exception e) {
				throw new Error("Error while processing serial event: " + e);
			}
		}
	}


//	/**
//	 * Set number of bytes to buffer before calling serialEvent()
//	 * in the host applet.
//	 */
//	public void buffer(int count) {
//		bufferUntil = false;
//		bufferSize = count;
//	}
//
//
//	/**
//	 * Set a specific byte to buffer until before calling
//	 * serialEvent() in the host applet.
//	 */
//	public void bufferUntil(int what) {
//		bufferUntil = true;
//		bufferUntilByte = what;
//	}


	/**
	 * Returns the number of bytes that have been read from serial
	 * and are waiting to be dealt with by the user.
	 */
	public int available() {
		return (_bufferLast - _bufferIndex);
	}


	/**
	 * Ignore all the bytes read so far and empty the buffer.
	 */
	public void clear() {
		_bufferLast = 0;
		_bufferIndex = 0;
	}


	/**
	 * Returns a number between 0 and 255 for the next byte that's
	 * waiting in the buffer.
	 * Returns -1 if there was no byte (although the user should
	 * first check available() to see if things are ready to avoid this)
	 */
	public int read() {
		if (_bufferIndex == _bufferLast) return -1;

		synchronized (_buffer) {
			int outgoing = _buffer[_bufferIndex++] & 0xff;
			if (_bufferIndex == _bufferLast) {  // rewind
				_bufferIndex = 0;
				_bufferLast = 0;
			}
			return outgoing;
		}
	}


	/**
	 * Same as read() but returns the very last value received
	 * and clears the buffer. Useful when you just want the most
	 * recent value sent over the port.
	 */
	public int last() {
		if (_bufferIndex == _bufferLast) return -1;
		synchronized (_buffer) {
			int outgoing = _buffer[_bufferLast-1];
			_bufferIndex = 0;
			_bufferLast = 0;
			return outgoing;
		}
	}


	/**
	 * Returns the next byte in the buffer as a char.
	 * Returns -1, or 0xffff, if nothing is there.
	 */
	public char readChar() {
		if (_bufferIndex == _bufferLast) return (char)(-1);
		return (char) read();
	}


	/**
	 * Just like last() and readChar().
	 */
	public char lastChar() {
		if (_bufferIndex == _bufferLast) return (char)(-1);
		return (char) last();
	}


	/**
	 * Return a byte array of anything that's in the serial buffer.
	 * Not particularly memory/speed efficient, because it creates
	 * a byte array on each read, but it's easier to use than
	 * readBytes(byte b[]) (see below).
	 */
	public byte[] readBytes() {
		if (_bufferIndex == _bufferLast) return null;

		synchronized (_buffer) {
			int length = _bufferLast - _bufferIndex;
			byte outgoing[] = new byte[length];
			System.arraycopy(_buffer, _bufferIndex, outgoing, 0, length);

			_bufferIndex = 0;  // rewind
			_bufferLast = 0;
			return outgoing;
		}
	}


	/**
	 * Grab whatever is in the serial buffer, and stuff it into a
	 * byte buffer passed in by the user. This is more memory/time
	 * efficient than readBytes() returning a byte[] array.
	 *
	 * Returns an int for how many bytes were read. If more bytes
	 * are available than can fit into the byte array, only those
	 * that will fit are read.
	 */
	public int readBytes(byte outgoing[]) {
		if (_bufferIndex == _bufferLast) return 0;

		synchronized (_buffer) {
			int length = _bufferLast - _bufferIndex;
			if (length > outgoing.length) length = outgoing.length;
			System.arraycopy(_buffer, _bufferIndex, outgoing, 0, length);

			_bufferIndex += length;
			if (_bufferIndex == _bufferLast) {
				_bufferIndex = 0;  // rewind
				_bufferLast = 0;
			}
			return length;
		}
	}


	/**
	 * Reads from the serial port into a buffer of bytes up to and
	 * including a particular character. If the character isn't in
	 * the serial buffer, then 'null' is returned.
	 */
	public byte[] readBytesUntil(int interesting) {
		if (_bufferIndex == _bufferLast) return null;
		byte what = (byte)interesting;

		synchronized (_buffer) {
			int found = -1;
			for (int k = _bufferIndex; k < _bufferLast; k++) {
				if (_buffer[k] == what) {
					found = k;
					break;
				}
			}
			if (found == -1) return null;

			int length = found - _bufferIndex + 1;
			byte outgoing[] = new byte[length];
			System.arraycopy(_buffer, _bufferIndex, outgoing, 0, length);

			_bufferIndex = 0;  // rewind
			_bufferLast = 0;
			return outgoing;
		}
	}


	/**
	 * Reads from the serial port into a buffer of bytes until a
	 * particular character. If the character isn't in the serial
	 * buffer, then 'null' is returned.
	 *
	 * If outgoing[] is not big enough, then -1 is returned,
	 *   and an error message is printed on the console.
	 * If nothing is in the buffer, zero is returned.
	 * If 'interesting' byte is not in the buffer, then 0 is returned.
	 */
	public int readBytesUntil(int interesting, byte outgoing[]) {
		if (_bufferIndex == _bufferLast) return 0;
		byte what = (byte)interesting;

		synchronized (_buffer) {
			int found = -1;
			for (int k = _bufferIndex; k < _bufferLast; k++) {
				if (_buffer[k] == what) {
					found = k;
					break;
				}
			}
			if (found == -1) return 0;

			int length = found - _bufferIndex + 1;
			if (length > outgoing.length) {
				System.err.println("readBytesUntil() byte buffer is" +
						" too small for the " + length +
						" bytes up to and including char " + interesting);
				return -1;
			}
			//byte outgoing[] = new byte[length];
			System.arraycopy(_buffer, _bufferIndex, outgoing, 0, length);

			_bufferIndex += length;
			if (_bufferIndex == _bufferLast) {
				_bufferIndex = 0;  // rewind
				_bufferLast = 0;
			}
			return length;
		}
	}


	/**
	 * Return whatever has been read from the serial port so far
	 * as a String. It assumes that the incoming characters are ASCII.
	 *
	 * If you want to move Unicode data, you can first convert the
	 * String to a byte stream in the representation of your choice
	 * (i.e. UTF8 or two-byte Unicode data), and send it as a byte array.
	 */
	public String readString() {
		if (_bufferIndex == _bufferLast) return null;
		return new String(readBytes());
	}


	/**
	 * Combination of readBytesUntil and readString. See caveats in
	 * each function. Returns null if it still hasn't found what
	 * you're looking for.
	 *
	 * If you want to move Unicode data, you can first convert the
	 * String to a byte stream in the representation of your choice
	 * (i.e. UTF8 or two-byte Unicode data), and send it as a byte array.
	 */
	public String readStringUntil(int interesting) {
		byte b[] = readBytesUntil(interesting);
		if (b == null) return null;
		return new String(b);
	}


	/**
	 * This will handle both ints, bytes and chars transparently.
	 * @throws IOException 
	 */
	public void write(int what) throws IOException {  // will also cover char
		_output.write(what & 0xff);  // for good measure do the &
		_output.flush();   // hmm, not sure if a good idea
	}


	public void write(byte bytes[]) throws IOException {
		_output.write(bytes);
		_output.flush();   // hmm, not sure if a good idea
	}


	/**
	 * Write a String to the output. Note that this doesn't account
	 * for Unicode (two bytes per char), nor will it send UTF8
	 * characters.. It assumes that you mean to send a byte buffer
	 * (most often the case for networking and serial i/o) and
	 * will only use the bottom 8 bits of each char in the string.
	 * (Meaning that internally it uses String.getBytes)
	 *
	 * If you want to move Unicode data, you can first convert the
	 * String to a byte stream in the representation of your choice
	 * (i.e. UTF8 or two-byte Unicode data), and send it as a byte array.
	 * @throws IOException 
	 */
	public void write(String what) throws IOException {
		write(what.getBytes());
	}


	/**
	 * If this just hangs and never completes on Windows,
	 * it may be because the DLL doesn't have its exec bit set.
	 * Why the hell that'd be the case, who knows.
	 * @throws Exception 
	 */
	static public String[] serialPortList() throws Exception {
		Vector list = new Vector();
		try {
			Enumeration portList = CommPortIdentifier.getPortIdentifiers();
			while (portList.hasMoreElements()) {
				CommPortIdentifier portId =
					(CommPortIdentifier) portList.nextElement();

				if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
					String name = portId.getName();
					list.addElement(name);
				}
			}

		} catch (UnsatisfiedLinkError e) {
			throw new Exception("Unsatisfied link error.");
		}
		String outgoing[] = new String[list.size()];
		list.copyInto(outgoing);
		return outgoing;
	}
//
//	public void ownershipChange(int type) {
//		System.out.println("Ownership changed to " + type);
//		if (type == PORT_OWNERSHIP_REQUESTED)
//			if (port != null) {
//				System.out.println("Closing port");
//				port.close();
//				port = null;
//			}
//	}

}
