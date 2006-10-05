package drone.core;

import net.infonode.docking.View;

public interface DockedExtension {
	enum Position {UPPER_LEFT, UPPER_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT}
	
	Position position();
	void init(View view);
}
