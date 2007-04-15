package drone.librarybrowser;

import javax.swing.tree.DefaultMutableTreeNode;

import ptolemy.kernel.Entity;

public class EntityTreeNode extends DefaultMutableTreeNode {

	public EntityTreeNode(Entity entity) {
		_entity = entity;
	}
		
	public Entity getEntity() {
		return _entity;
	}


	public String toString() {
		return _entity.getName();
	}
	
	private Entity _entity;
	
}
