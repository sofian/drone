package drone.core;

import ptolemy.kernel.Entity;

import com.trolltech.qt.gui.QTreeWidgetItem;
import com.trolltech.qt.gui.QTreeWidget;

public class EntityTreeWidgetItem extends QTreeWidgetItem {

	public EntityTreeWidgetItem(Entity entity) {
		_init(entity);
	}
	
	public EntityTreeWidgetItem(QTreeWidget parent, Entity entity) {
		super(parent);
		_init(entity);
	}

	public EntityTreeWidgetItem(QTreeWidgetItem parent, Entity entity) {
		super(parent);
		_init(entity);
	}
	
	public Entity getEntity() {
		return _entity;
	}

	private void _init(Entity entity) {
		_entity = entity;
		super.setText(0, entity.getName());
	}
	
	private Entity _entity;
	
}
