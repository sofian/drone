package drone.serial.actor;

import java.io.IOException;

import drone.serial.lib.Serial;
import ptolemy.actor.lib.Sink;
import ptolemy.actor.lib.Source;
import ptolemy.data.BooleanToken;
import ptolemy.data.IntToken;
import ptolemy.data.StringToken;
import ptolemy.data.Token;
import ptolemy.data.expr.Parameter;
import ptolemy.data.expr.StringParameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

@SuppressWarnings("serial")
public class SerialReader extends Source {

	public SerialReader(CompositeEntity container, String name)
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
		
		output.setTypeEquals(BaseType.GENERAL); // allows for string and int

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
		if (((BooleanToken)stringMode.getToken()).booleanValue()) {
			output.send(0, new StringToken(_serial.readStringUntil('\n')));
		} else {
			int val = _serial.read();
			if (val != -1)
				output.send(0, new IntToken(val));
		}
	}
	
	Serial _serial;
	
	public StringParameter serialName;
	public Parameter rate;
	public Parameter stringMode;
}
