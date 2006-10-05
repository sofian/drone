package drone.core;

import java.awt.BorderLayout;
import java.net.URL;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTree;

import net.infonode.docking.DockingWindow;
import net.infonode.docking.RootWindow;
import net.infonode.docking.SplitWindow;
import net.infonode.docking.TabWindow;
import net.infonode.docking.View;
import net.infonode.docking.WindowBar;
import net.infonode.docking.util.DockingUtil;
import net.infonode.docking.util.ViewMap;
import net.infonode.util.Direction;

import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.Entity;
import ptolemy.moml.MoMLParser;

public class MainWindow extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public static final int UPPER_LEFT_VIEW = 0;
	public static final int BOTTOM_LEFT_VIEW = 1;
	public static final int UPPER_RIGHT_VIEW = 2;
	public static final int BOTTOM_RIGHT_VIEW = 3;
	
	public MainWindow() throws Exception {
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
		ViewMap viewMap = new ViewMap();
		viewMap.addView(UPPER_LEFT_VIEW, _upperLeftView = new View("Upper left", null, new JLabel("This is upper left view")));
		viewMap.addView(BOTTOM_LEFT_VIEW, _bottomLeftView = new View("Bottom  left", null, new JLabel("This is bottom left view")));
		viewMap.addView(UPPER_RIGHT_VIEW, _upperRightView = new View("Upper right", null, new JLabel("This is upper right view")));
		viewMap.addView(BOTTOM_RIGHT_VIEW, _bottomRightView = new View("Bottom right", null, new JLabel("This is bottom right view")));
		_rootWindow = DockingUtil.createRootWindow(viewMap, true); 
	}

	/**
	 * Sets the default window layout.
	 * @throws Exception 
	 */
	private void setDefaultLayout() throws Exception {
		_rootWindow.setWindow(
				new SplitWindow(true,
								0.25f,
								new SplitWindow(false,
												0.5f,
												_upperLeftView,
												_bottomLeftView),
								new SplitWindow(false,
												0.75f,
												_upperRightView,
												_bottomRightView))
		);
		// Open the configuration.
		Configuration configuration = (Configuration) Configuration.configurations().iterator().next();
		if (configuration == null) {
			throw new Exception("There are no existing configurations.");
		}

		// Create the tree view of the actor library.
		CompositeEntity actorList = (CompositeEntity) configuration.getEntity("actor library");
////		QTreeView libraryTree = new EntityTreeWidget(actorList, dock);		
////		dock.setWidget(libraryTree);

		JTree libraryTree = new EntityTree(actorList);
		_upperLeftView.setComponent(libraryTree);
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

	private RootWindow _rootWindow;
	private View _upperLeftView;
	private View _bottomLeftView;
	private View _upperRightView;
	private View _bottomRightView;
	
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

}
