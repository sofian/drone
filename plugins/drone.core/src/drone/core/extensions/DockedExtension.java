package drone.core.extensions;

import javax.swing.JComponent;

public interface DockedExtension {
	enum Position {UPPER_LEFT, UPPER_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT}
	
	Position position();
	JComponent createComponent() throws FailToCreateComponentException;
}
