package drone.core;
/****************************************************************************
 **
 ** Copyright (C) 1992-2006 Trolltech ASA. All rights reserved.
 **
 ** This file is part of Qt Jambi.
 **
 ** $JAVA_LICENSE$
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/


import java.util.Iterator;

import ptolemy.kernel.CompositeEntity;
import ptolemy.kernel.Entity;

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

	    
		// Creation of the tree view.
		Configuration configuration = (Configuration) Configuration.configurations().iterator().next();
		if (configuration == null) {
			throw new Exception("There are no existing configurations.");
		}
		CompositeEntity actorList = (CompositeEntity) configuration.getEntity("actor library");
	    
		QTreeView libraryTree = new EntityTreeWidget(actorList);		
		leftTab.addTab(libraryTree, "Library");
	    
		
		//creation of Julien
		QTextEdit txtJulien = new QTextEdit();
		centerTab.addTab(txtJulien, "Julien");
		
		//creation of croquet
		QTextEdit txtCroquet = new QTextEdit();
		centerTab.addTab(txtCroquet, "Croquet");

		//creation of rag
		QTextEdit txtRag = new QTextEdit();
		bottomTab.addTab(txtRag, "Rag");
		
	}
	

}
