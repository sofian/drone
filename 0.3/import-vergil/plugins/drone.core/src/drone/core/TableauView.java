package drone.core;

import java.awt.Component;

import javax.swing.Icon;

import ptolemy.actor.gui.Tableau;

import net.infonode.docking.View;

@SuppressWarnings("serial")
public class TableauView extends View {

	public TableauView(Icon icon, Tableau tableau) {
		super(tableau.getTitle(), icon, tableau.getFrame().getComponent(0));
		// TODO Auto-generated constructor stub
		_tableau = tableau;
	}
	
	public Tableau getTableau() { return _tableau; }
	
	private Tableau _tableau;

}
