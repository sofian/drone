package drone.librarybrowser;

import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.Entity;

import java.util.ArrayList;
import java.util.ListIterator;

import javax.swing.JTree;
import javax.swing.tree.DefaultTreeModel;

public class EntityTree extends JTree {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public EntityTree(Entity root) {
		super();
		
		EntityTreeNode rootNode = new EntityTreeNode(root);
		_buildTree(rootNode);
		setModel(new DefaultTreeModel(rootNode));
		setRootVisible(false);
	}

	protected static ListIterator _childrenListIterator(Entity root) {
		if (root != null && root instanceof CompositeEntity)
			return ((CompositeEntity)root).entityList().listIterator();
		else
			return new ArrayList().listIterator();
	}
	
	protected static void _buildTree(EntityTreeNode currentRootNode) {
		if (currentRootNode != null) {
			
			Entity currentRoot = currentRootNode.getEntity();
			ListIterator i = _childrenListIterator(currentRoot);
			while (i.hasNext()) {
				EntityTreeNode child = new EntityTreeNode((Entity) i.next());
				_buildTree(child);
				currentRootNode.add(child);
			}
		}
	}

}