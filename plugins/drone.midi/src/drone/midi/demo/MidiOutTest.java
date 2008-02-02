package drone.midi.demo;

import java.util.Vector;

import javax.sound.midi.InvalidMidiDataException;
import javax.sound.midi.MidiDevice;
import javax.sound.midi.MidiSystem;
import javax.sound.midi.MidiUnavailableException;
import javax.sound.midi.Receiver;
import javax.sound.midi.ShortMessage;
import javax.sound.midi.Synthesizer;
import javax.sound.midi.MidiDevice.Info;

public class MidiOutTest {

	public static void main(String[] args) throws InvalidMidiDataException, MidiUnavailableException {
		// Obtain information about all the installed synthesizers.
		MidiDevice device = null;
		MidiDevice.Info[] infos = MidiSystem.getMidiDeviceInfo();
		if (args.length == 0) {
			for (int i = 0; i < infos.length; i++) {
				try {
					System.out.println("Infos[" + i + "] = " + infos[i].getDescription());
					device = MidiSystem.getMidiDevice(infos[i]);
				} catch (MidiUnavailableException e) {
					// Handle or throw exception...
				}
			}
		} else {
			int index = Integer.parseInt(args[0]);
			device = MidiSystem.getMidiDevice(infos[index]);
			device.open();
			ShortMessage myMsg = new ShortMessage();
			int note = Integer.parseInt(args[1]);
			int velocity = Integer.parseInt(args[2]);
			myMsg.setMessage(ShortMessage.NOTE_ON, note, velocity);
			device.getReceiver().send(myMsg, -1);
		}
	}
}
