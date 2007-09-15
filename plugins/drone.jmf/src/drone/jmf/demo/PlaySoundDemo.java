package drone.jmf.demo;

import java.io.IOException;

import javax.media.Controller;
import javax.media.ControllerEvent;
import javax.media.ControllerListener;
import javax.media.MediaLocator;
import javax.media.NoDataSourceException;
import javax.media.NoPlayerException;
import javax.media.Player;
import javax.media.Time;
import javax.media.TimeBase;

import net.sf.fmj.ejmf.toolkit.util.SourcedTimer;
import net.sf.fmj.ejmf.toolkit.util.StateWaiter;
import net.sf.fmj.ui.application.CaptureDeviceBrowser;
import net.sf.fmj.ui.application.ContainerPlayerStatusListener;

public class PlaySoundDemo implements ControllerListener {
	
	private Player player;
	
	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		PlaySoundDemo demo = new PlaySoundDemo();
		demo.setMediaLocation(args[0]);
		demo.start();
	}
	
	/**
	 * 
	 */
	public void stop() {
		if (player != null) {
			player.stop();
		}
	}
	
	private void createNewPlayer(MediaLocator source) throws NoPlayerException, IOException {
		player = javax.media.Manager.createPlayer(source);
		player.realize();

		// copied from EJMF StandardStartControl
	    final int state = player.getState();

	    if (state == Controller.Started) 
	    	return;

	    if (state < Controller.Prefetched) {
			StateWaiter w = new StateWaiter(player);
			w.blockingPrefetch();
	    }

	    final TimeBase tb = player.getTimeBase();
	    player.syncStart(tb.getTime());
	    
	    player.getGainControl();
	}
	
	/**
	 * 
	 */
	public void start() {
		if (player != null) {
			player.start();
		}
	}

	/**
	 * Deallocate all resources used by the player.
	 */
	public void deallocate() {
		if (player != null) {
			player.deallocate();
			player = null;
		}
	}

	/**
	 * Deallocate resources and close the player.
	 */
	public void close() {
		if (player != null) {
			player.close();
		}
	}

	/**
	 * Set the current media time of the content.
	 */
	public void setMediaTime(Time time) {
		if (player != null) {
			player.setMediaTime(time);
		}
	}

	public void setMediaLocation(String mediaLocation) throws Exception {
		
		try
		{
			MediaLocator locator = new MediaLocator(mediaLocation);
			stop();
			if (player != null) {
				player.close();
				player.deallocate();
				player = null;
			}
			createNewPlayer(locator);
		} catch (Exception e) {
			e.printStackTrace();
			throw e;
		}
		
	}

	public void controllerUpdate(ControllerEvent event) {
	}
	
}
