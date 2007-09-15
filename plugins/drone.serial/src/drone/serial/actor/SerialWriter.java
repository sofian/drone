package drone.serial.actor;

import java.io.IOException;

import drone.serial.lib.Serial;
import ptolemy.actor.lib.Sink;
import ptolemy.actor.lib.Source;
import ptolemy.data.StringToken;
import ptolemy.data.Token;
import ptolemy.data.expr.Parameter;
import ptolemy.data.expr.StringParameter;
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
	}
	
	public void initialize() throws IllegalActionException {
		try {
			_serial = Serial.getSerial(((StringToken)serialName.getToken()).stringValue());
		} catch (Exception e) {
			throw new IllegalActionException(this, e, 
								"Problem while initializing serial communication.");
		}
	}
	
	public void fire() throws IllegalActionException {
		super.fire();
		if (input.getWidth() > 0 && input.hasToken(0)) {
			StringToken s = StringToken.convert(input.get(0));
			String str = s.stringValue().trim() + '\n';
			try {
				_serial.write(str);
			} catch (IOException e) {
				throw new IllegalActionException(this, e, "Error while writing");
			}
		}
	}
	
	Serial _serial;
	
	public StringParameter serialName;
	public Parameter rate;
}
