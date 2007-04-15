package drone.core;


import org.java.plugin.boot.Application;
import org.java.plugin.boot.ApplicationPlugin;
import org.java.plugin.util.ExtendedProperties;

public final class CorePlugin extends ApplicationPlugin {
    /**
     * This plug-in ID.
     */
    public static final String PLUGIN_ID = "drone.core";

	protected Application initApplication(ExtendedProperties arg0, String[] arg1) throws Exception {
		return new CoreApplication(arg0.getProperty("basePath"), arg1, this);
	}

	protected void doStart() throws Exception {
	}

	protected void doStop() throws Exception {
	}

}
