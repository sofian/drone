package drone.core;

import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.Entity;

import java.util.ArrayList;
import java.util.ListIterator;

import com.trolltech.qt.gui.QTreeWidget;
import com.trolltech.qt.gui.QWidget;

public class EntityTreeWidget extends QTreeWidget {
	
	public EntityTreeWidget(Entity root, QWidget parent) {
		super(parent);
		super.setColumnCount(1);
		
		ListIterator i = _childrenListIterator(root);
		while (i.hasNext()) {
			EntityTreeWidgetItem currentRoot = new EntityTreeWidgetItem(this, (Entity) i.next());
			_buildTree(currentRoot);
		}
		super.setHeaderItem(new EntityTreeWidgetItem(root));
	}

	protected static ListIterator _childrenListIterator(Entity root) {
		if (root != null && root instanceof CompositeEntity)
			return ((CompositeEntity)root).entityList().listIterator();
		else
			return new ArrayList().listIterator();
	}
	
	protected static void _buildTree(EntityTreeWidgetItem currentRootItem) {
		if (currentRootItem != null) {
			
			Entity currentRoot = currentRootItem.getEntity();
			ListIterator i = _childrenListIterator(currentRoot);
			while (i.hasNext()) {
				EntityTreeWidgetItem child = new EntityTreeWidgetItem(currentRootItem, (Entity) i.next());
				_buildTree(child);
			}
		}
	}

}