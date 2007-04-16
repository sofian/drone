package drone.spikes.tinygui;

import org.java.plugin.boot.Application;
import org.java.plugin.boot.ApplicationPlugin;
import org.java.plugin.util.ExtendedProperties;

public final class CorePlugin extends ApplicationPlugin {
    /**
     * This plug-in ID.
     */
    public static final String PLUGIN_ID = "drone.core";

	@Override
	protected Application initApplication(ExtendedProperties arg0, String[] arg1) throws Exception {
		// TODO Auto-generated method stub
		return new CoreApplication(arg0.getProperty("basePath"), arg1);
	}

	@Override
	protected void doStart() throws Exception {
		// TODO Auto-generated method stub
		System.out.println("START");
	}

	@Override
	protected void doStop() throws Exception {
		// TODO Auto-generated method stub
		System.out.println("STOP");
	}

}
