package drone.librarybrowser;

import javax.swing.JComponent;
import javax.swing.JTree;

import ptolemy.kernel.CompositeEntity;

import net.infonode.docking.View;
import drone.core.Configuration;
import drone.core.extensions.ViewExtension;
import drone.core.extensions.FailToCreateComponentException;

public class LibraryBrowserViewExtension implements ViewExtension {

	public Position position() {
		return Position.UPPER_LEFT;
	}

	public JComponent createComponent() throws FailToCreateComponentException {
		// Create the tree view of the actor library.
		// Open the configuration.

		Configuration configuration = (Configuration) Configuration.configurations().iterator().next();
		if (configuration == null) {
			throw new FailToCreateComponentException("There are no existing configurations.");
		}

		CompositeEntity actorList = (CompositeEntity) configuration.getEntity("actor library");
		return new EntityTree(actorList);
	}

}
