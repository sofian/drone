package drone.serial.actor;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.Charset;

import drone.serial.lib.Serial;
import ptolemy.actor.lib.Sink;
import ptolemy.actor.lib.Source;
import ptolemy.data.BooleanToken;
import ptolemy.data.ScalarToken;
import ptolemy.data.StringToken;
import ptolemy.data.Token;
import ptolemy.data.expr.Parameter;
import ptolemy.data.expr.StringParameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

@SuppressWarnings("serial")
public class SerialWriter extends Sink {

	public SerialWriter(CompositeEntity container, String name)
			throws NameDuplicationException, IllegalActionException {
		super(container, name);
		
		serialName = new StringParameter(this, "serialName");
		String[] serialPortList;
		try {
			serialPortList = Serial.serialPortList();
		} catch (Exception e) {
			throw new IllegalActionException(this, e, 
			"Problem while loading serial port list.");
		}
		for (int i=0; i<serialPortList.length; i++)
			serialName.addChoice(serialPortList[i]);
		if (serialPortList.length > 0)
			serialName.setExpression(serialPortList[0]);
		else
			throw new IllegalActionException(this, "No serial ports available.");
		
		stringMode = new Parameter(this, "stringMode");
		stringMode.setTypeEquals(BaseType.BOOLEAN);
		stringMode.setExpression("true");
		
		input.setTypeEquals(BaseType.GENERAL);
	}
	
	public void initialize() throws IllegalActionException {
		try {
			_serial = Serial.getSerial(((StringToken)serialName.getToken()).stringValue());
		} catch (Exception e) {
			throw new IllegalActionException(this, e, 
								"Problem while initializing serial communication.");
		}
	}
	
	private byte[] getBytes (char[] chars) {
		Charset cs = Charset.forName ("UTF-8");
		CharBuffer cb = CharBuffer.allocate (chars.length);
		cb.put (chars);
        cb.flip ();
		ByteBuffer bb = cs.encode (cb);
		
		return bb.array();
    }
	
	public void fire() throws IllegalActionException {
		super.fire();
		if (input.getWidth() > 0 && input.hasToken(0)) {
			try {
				if (((BooleanToken)stringMode.getToken()).booleanValue()) {
					StringToken s = StringToken.convert(input.get(0));
					String str = s.stringValue().trim() + '\n';
					_serial.write(str);
				} else {
					Token token = input.get(0);
					if (token instanceof ScalarToken)
						_serial.write(((ScalarToken)token).intValue());
					else if (token instanceof StringToken)
						_serial.write( getBytes( ((StringToken)token).stringValue().toCharArray() ));
					else
						throw new IllegalActionException(this, "Wrong token type " + token.getClass().toString());
				}
			} catch (IOException e) {
				throw new IllegalActionException(this, e, "Error while writing");
			}
		}
	}
	
	Serial _serial;
	
	public StringParameter serialName;
	public Parameter rate;
	public Parameter stringMode;
}
