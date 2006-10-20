package drone.core;

import java.awt.BorderLayout;
import java.awt.MenuBar;
import java.net.URL;
import java.util.List;
import java.util.ListIterator;

import javax.swing.Icon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JTree;

import net.infonode.docking.DockingWindow;
import net.infonode.docking.RootWindow;
import net.infonode.docking.SplitWindow;
import net.infonode.docking.TabWindow;
import net.infonode.docking.View;
import net.infonode.docking.ViewSerializer;
import net.infonode.docking.WindowBar;
import net.infonode.docking.properties.RootWindowProperties;
import net.infonode.docking.theme.DockingWindowsTheme;
import net.infonode.docking.theme.ShapedGradientDockingTheme;
import net.infonode.docking.util.DockingUtil;
import net.infonode.docking.util.StringViewMap;
import net.infonode.docking.util.ViewMap;
import net.infonode.util.Direction;

import ptolemy.actor.gui.Tableau;
import ptolemy.gui.Top;
import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.Entity;
import ptolemy.moml.MoMLParser;

import drone.core.extensions.ViewExtension;
import drone.core.extensions.FailToCreateComponentException;

public class MainWindow extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public static final float DEFAULT_LEFT_PROPORTION = 0.25f;
	public static final float DEFAULT_UPPER_LEFT_PROPORTION = 0.5f;
	public static final float DEFAULT_UPPER_RIGHT_PROPORTION = 0.75f;
	
	private MainWindow() throws Exception {
//		createMenus();
		createRootWindow();
		setDefaultLayout();
		showFrame();
//		setWindowTitle(tr("Drone"));

//		QTabWidget leftTab = new QTabWidget(this);
//		QTabWidget centerTab = new QTabWidget(this);
//		QTabWidget bottomTab = new QTabWidget(this);

//		QSplitter vsplitter = new QSplitter(Qt.Horizontal,this);
//		QSplitter hsplitter = new QSplitter(Qt.Vertical, this);

//		vsplitter.addWidget(leftTab);
//		vsplitter.addWidget(hsplitter);
//		hsplitter.addWidget(centerTab);
//		hsplitter.addWidget(bottomTab);

//		setCentralWidget(vsplitter);
//		//layout based on 800x600 resolution
//		leftTab.setSizePolicy(75, 800);
//		centerTab.setSizePolicy(725, 500);
//		bottomTab.setSizePolicy(725, 100);


//		// Open the configuration.
//		Configuration configuration = (Configuration) Configuration.configurations().iterator().next();
//		if (configuration == null) {
//		throw new Exception("There are no existing configurations.");
//		}

//		// Create the tree view of the actor library.
//		CompositeEntity actorList = (CompositeEntity) configuration.getEntity("actor library");
////		QTreeView libraryTree = new EntityTreeWidget(actorList, dock);		
////		dock.setWidget(libraryTree);

//		QTreeView libraryTree = new EntityTreeWidget(actorList);		
//		leftTab.addTab(libraryTree, "Library");

//		// Create the tree view of the opened model.
////		MoMLParser parser = new MoMLParser();
////		URL url = new URL("file:/Users/tats/Documents/workspace/drone/data/demos/testMath.xml");
////		Entity myEntity = (Entity) parser.parse(url, url);
////		QTreeView projectTree = new EntityTreeWidget(myEntity, dock);
////		dock.setWidget(projectTree);
////		Tableau modelTableau = configuration.openModel(url, url, url.toExternalForm());


//		// Add the dock widget to the view.
////		addDockWidget(QDockWidget.AllDockWidgetFeatures, dock);

//		//creation of Julien
//		QTextEdit txtJulien = new QTextEdit();
//		centerTab.addTab(txtJulien, "Julien");

//		//creation of croquet
//		QTextEdit txtCroquet = new QTextEdit();
//		centerTab.addTab(txtCroquet, "Croquet");

//		//creation of rag
//		QTextEdit txtRag = new QTextEdit();
//		bottomTab.addTab(txtRag, "Rag");


//		buildMenu();
	}

	/**
	 * Creates the root window and the views.
	 */
	private void createRootWindow() {
		_viewMap = new StringViewMap();
		_upperLeftTabWindow = new TabWindow();
		_bottomLeftTabWindow = new TabWindow();
		_upperRightTabWindow = new TabWindow();
		_bottomRightTabWindow = new TabWindow();
		_rootWindow = new RootWindow(_viewMap);
		
		// Set gradient theme. The theme properties object is the super object of our properties object, which
	    // means our property value settings will override the theme values
		DockingWindowsTheme shapedGradientDockingTheme = new ShapedGradientDockingTheme();	    
		_rootWindowProperties.addSuperObject(shapedGradientDockingTheme.getRootWindowProperties());

	    // Our properties object is the super object of the root window properties object, so all property values of the
	    // theme and in our property object will be used by the root window
	    _rootWindow.getRootWindowProperties().addSuperObject(_rootWindowProperties);

	    // Enable the bottom window bar
	    _rootWindow.getWindowBar(Direction.DOWN).setEnabled(true);
	}

	public void addDockedExtension(String label, ViewExtension extension) {
		ViewExtension.Position position = extension.position();
		View view = null;
		try {
			view = new View(label, null, extension.createComponent());
		} catch (FailToCreateComponentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//TODO: must have unique naming for window view name, for serialization
		_viewMap.addView(label, view);
		if (position == ViewExtension.Position.UPPER_LEFT) {
			_upperLeftTabWindow.addTab(view);
		} else if (position == ViewExtension.Position.BOTTOM_LEFT) {
			_bottomLeftTabWindow.addTab(view);
		} else if (position == ViewExtension.Position.UPPER_RIGHT) {
			_upperRightTabWindow.addTab(view);
		} else {
			_bottomRightTabWindow.addTab(view);
		}
	}
	
	public void showTableau(Tableau tableau, ViewExtension.Position position) {
		View view = null;
		JFrame tableauFrame = tableau.getFrame();
		if (tableauFrame instanceof Top) {
			((Top)tableauFrame).setCentering(false);
		}
		tableauFrame.pack();
		JMenuBar menuBar = tableauFrame.getJMenuBar();
		menuBar.setVisible(true);
		setJMenuBar(menuBar);
		view = new View(tableau.getTitle(), null, tableau.getFrame().getComponent(0));
		//TODO: must have unique naming for window view name, for serialization
		_viewMap.addView(tableau.getTitle(), view);
		if (position == ViewExtension.Position.UPPER_LEFT) {
			_upperLeftTabWindow.addTab(view);
		} else if (position == ViewExtension.Position.BOTTOM_LEFT) {
			_bottomLeftTabWindow.addTab(view);
		} else if (position == ViewExtension.Position.UPPER_RIGHT) {
			_upperRightTabWindow.addTab(view);
		} else {
			_bottomRightTabWindow.addTab(view);
		}
		
	}
	
	/**
	 * Sets the default window layout.
	 * @throws Exception 
	 */
	private void setDefaultLayout() throws Exception {
		
//		_upperLeftTabWindow.getChildWindowCount();
//		i = extensionList.iterator();
//		
//		for (; it.hasNext(); it.next())
		_rootWindow.setWindow(
				new SplitWindow(true,
								DEFAULT_LEFT_PROPORTION,
								new SplitWindow(false,
												DEFAULT_UPPER_LEFT_PROPORTION,
												_upperLeftTabWindow,
												_bottomLeftTabWindow),
								new SplitWindow(false,
												DEFAULT_UPPER_RIGHT_PROPORTION,
												_upperRightTabWindow,
												_bottomRightTabWindow))
		);

	}

	/**
	 * Initializes the frame and shows it.
	 */
	private void showFrame() {
//		frame.getContentPane().add(createToolBar(), BorderLayout.NORTH);
		getContentPane().add(_rootWindow, BorderLayout.CENTER);
		//setJMenuBar(createMenuBar());
		setSize(900, 700);
		setVisible(true);
	}

	private void createMenus() {
		// Create the "File" menu.
		JMenu fileMenu = new JMenu("File");
		JMenuItem newMenuItem = new JMenuItem("New");
		JMenuItem openMenuItem = new JMenuItem("Open");
		JMenuItem closeMenuItem = new JMenuItem("Close");
		fileMenu.add(newMenuItem);
		fileMenu.add(openMenuItem);
		fileMenu.add(closeMenuItem);
		
		// Create the "Edit" menu.
		JMenu editMenu = new JMenu("Edit");
		JMenuItem cutMenuItem = new JMenuItem("Cut");
		JMenuItem copyMenuItem = new JMenuItem("Copy");
		JMenuItem pasteMenuItem = new JMenuItem("Paste");
		JMenuItem deleteMenuItem = new JMenuItem("Delete");
		editMenu.add(cutMenuItem);
		editMenu.add(copyMenuItem);
		editMenu.add(pasteMenuItem);
		editMenu.add(deleteMenuItem);
		
		// Create the menu bar.
		JMenuBar menuBar = new JMenuBar();
		menuBar.add(fileMenu);
		menuBar.add(editMenu);
		menuBar.setVisible(true);
		setJMenuBar(menuBar);
	}
	
	private RootWindow _rootWindow;
	private StringViewMap _viewMap;
	private TabWindow _upperLeftTabWindow;
	private TabWindow _bottomLeftTabWindow;
	private TabWindow _upperRightTabWindow;
	private TabWindow _bottomRightTabWindow;
	  /**
	   * In this properties object the modified property values for close buttons etc. are stored. This object is cleared
	   * when the theme is changed.
	   */
	private RootWindowProperties _rootWindowProperties = new RootWindowProperties();

	
//	protected void buildMenu() {
//	// Create file menu.
//	QMenu fileMenu = menuBar().addMenu(tr("File"));

//	QAction newAct = new QAction(tr("&New"), this);
//	newAct.setShortcut(tr("Ctrl+N"));
//	newAct.setStatusTip(tr("Create a new file"));
//	fileMenu.addAction(newAct);

//	QAction openAct = new QAction(tr("&Open..."), this);
//	openAct.setShortcut(tr("Ctrl+O"));
//	openAct.setStatusTip(tr("Open an existing file"));
//	fileMenu.addAction(openAct);

//	// Create edit menu.
//	QMenu editMenu = menuBar().addMenu(tr("Edit"));
//	}
	public static MainWindow instance() { 
		if (_instance == null) {
			try {
				_instance = new MainWindow();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}	
		return _instance; 
	}

	private static MainWindow _instance = null;
}

