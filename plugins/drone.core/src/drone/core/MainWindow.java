package drone.core;

import java.net.URL;

import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.Entity;
import ptolemy.moml.MoMLParser;

import com.trolltech.qt.core.Qt;
import com.trolltech.qt.gui.*;

public class MainWindow extends QMainWindow {

	public MainWindow() throws Exception {
		setWindowTitle(tr("Drone"));
				
		QTabWidget leftTab = new QTabWidget(this);
		QTabWidget centerTab = new QTabWidget(this);
		QTabWidget bottomTab = new QTabWidget(this);
						
		QSplitter vsplitter = new QSplitter(Qt.Horizontal,this);
		QSplitter hsplitter = new QSplitter(Qt.Vertical, this);

	    vsplitter.addWidget(leftTab);
	    vsplitter.addWidget(hsplitter);
	    hsplitter.addWidget(centerTab);
	    hsplitter.addWidget(bottomTab);
	    
	    setCentralWidget(vsplitter);
		//layout based on 800x600 resolution
	    leftTab.setSizePolicy(75, 800);
		centerTab.setSizePolicy(725, 500);
		bottomTab.setSizePolicy(725, 100);

	    
		// Open the configuration.
		Configuration configuration = (Configuration) Configuration.configurations().iterator().next();
		if (configuration == null) {
			throw new Exception("There are no existing configurations.");
		}
		
		// Create the tree view of the actor library.
		CompositeEntity actorList = (CompositeEntity) configuration.getEntity("actor library");
//		QTreeView libraryTree = new EntityTreeWidget(actorList, dock);		
//		dock.setWidget(libraryTree);
	    
		QTreeView libraryTree = new EntityTreeWidget(actorList);		
		leftTab.addTab(libraryTree, "Library");
	    
		// Create the tree view of the opened model.
//		MoMLParser parser = new MoMLParser();
//		URL url = new URL("file:/Users/tats/Documents/workspace/drone/data/demos/testMath.xml");
//		Entity myEntity = (Entity) parser.parse(url, url);
//		QTreeView projectTree = new EntityTreeWidget(myEntity, dock);
//		dock.setWidget(projectTree);
//		Tableau modelTableau = configuration.openModel(url, url, url.toExternalForm());

		
		// Add the dock widget to the view.
//		addDockWidget(QDockWidget.AllDockWidgetFeatures, dock);

		//creation of Julien
		QTextEdit txtJulien = new QTextEdit();
		centerTab.addTab(txtJulien, "Julien");
		
		//creation of croquet
		QTextEdit txtCroquet = new QTextEdit();
		centerTab.addTab(txtCroquet, "Croquet");

		//creation of rag
		QTextEdit txtRag = new QTextEdit();
		bottomTab.addTab(txtRag, "Rag");
		
		
		buildMenu();
	}
	
	protected void buildMenu() {
		// Create file menu.
		QMenu fileMenu = menuBar().addMenu(tr("File"));
		
		QAction newAct = new QAction(tr("&New"), this);
		newAct.setShortcut(tr("Ctrl+N"));
		newAct.setStatusTip(tr("Create a new file"));
		fileMenu.addAction(newAct);
		
		QAction openAct = new QAction(tr("&Open..."), this);
		openAct.setShortcut(tr("Ctrl+O"));
		openAct.setStatusTip(tr("Open an existing file"));
		fileMenu.addAction(openAct);
		
		// Create edit menu.
		QMenu editMenu = menuBar().addMenu(tr("Edit"));
	}

}
