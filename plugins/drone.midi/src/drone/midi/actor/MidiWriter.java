package drone.midi.actor;

import drone.midi.lib.MidiIO;
import drone.midi.lib.MidiOut;
import drone.midi.lib.Note;
import ptolemy.actor.lib.Sink;
import ptolemy.actor.parameters.PortParameter;
import ptolemy.data.IntToken;
import ptolemy.data.MatrixToken;
import ptolemy.data.StringToken;
import ptolemy.data.expr.StringParameter;
import ptolemy.data.type.BaseType;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.util.Attribute;
import ptolemy.kernel.util.IllegalActionException;
import ptolemy.kernel.util.NameDuplicationException;

@SuppressWarnings("serial")
public class MidiWriter extends Sink {

	public MidiWriter(CompositeEntity container, String name)
	throws NameDuplicationException, IllegalActionException {
		super(container, name);

		midiOutputName = new StringParameter(this, "midiOutputName");

		for (int i = 0; i < _midiIo.numberOfOutputDevices(); i++)
			midiOutputName.addChoice(_midiIo.getOutputDeviceName(i));
		if (_midiIo.numberOfOutputDevices() > 0)
			midiOutputName.setExpression(_midiIo.getOutputDeviceName(0));
		else
			throw new IllegalActionException(this, "No midi output ports available.");

		channel = new PortParameter(this, "channel");
		channel.setTypeEquals(BaseType.INT);
		channel.setExpression("0");

		input.setMultiport(true);
		input.setTypeEquals(BaseType.INT_MATRIX);
//		note = new TypedIOPort(this, "note");
//		note.setTypeEquals(BaseType.INT);

//		velocity = new TypedIOPort(this, "velocity");
//		velocity.setTypeEquals(BaseType.INT);

//		duration = new TypedIOPort(this, "duration");
//		duration.setTypeEquals(BaseType.INT);
	}

	public void attributeChanged(Attribute attribute)
	throws IllegalActionException {
		if (attribute == midiOutputName) {
			String newMidiOutputName = ((StringToken)midiOutputName.getToken()).stringValue();
			if (newMidiOutputName != _midiOutputName) {
				_midiOut = null;
				_midiOutputName = newMidiOutputName;
			}
		} else if (attribute == channel) {
			int newChannel = ((IntToken)channel.getToken()).intValue();
			if (newChannel != _channel) {
				_midiOut = null;
				_channel = newChannel;
			}
		}
		super.attributeChanged(attribute);
	}

	public boolean prefire() throws IllegalActionException {
		if (_midiOut == null)
			_midiOut = _midiIo.getMidiOut(_channel, _midiOutputName);
		return super.prefire();
	}

	public void fire() throws IllegalActionException {
		super.fire();
		for (int p=0; p<input.getWidth(); p++) {
			MatrixToken token = (MatrixToken)input.get(p);
			if (token.getRowCount() != 1 || token.getColumnCount() != 3)
				throw new IllegalActionException(this, "Input matrix " + p + " has wrong dimensions " 
						+ token.getRowCount() + "x" + token.getColumnCount() + ", should be 1x3.");
			int[] values = token.intMatrix()[0];
			_midiOut.sendNote(new Note(values[0], values[1], values[2]));
		}
	}

	MidiIO _midiIo = MidiIO.getInstance();
	MidiOut _midiOut = null;

	public StringParameter midiOutputName;

	public PortParameter channel;

	protected String _midiOutputName;
	protected int _channel;

//	public TypedIOPort note;
//	public TypedIOPort velocity;
//	public TypedIOPort duration;

}
