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

import com.trolltech.qt.gui.*;

public class MainWindow extends QMainWindow {
	
    public MainWindow() throws Exception {
        setWindowTitle(tr("Drone"));
        QDockWidget dock = new QDockWidget(this);
        QTextEdit textEdit = new QTextEdit(dock);
        dock.setWidget(textEdit);
        
        addDockWidget(QDockWidget.AllDockWidgetFeatures, dock);
        Configuration configuration = (Configuration) Configuration.configurations().iterator().next();
        if (configuration == null) {
        	throw new Exception("There are no existing configurations.");
        }
        CompositeEntity actorList = (CompositeEntity) configuration.getEntity("actor library");
        Iterator i = actorList.deepEntityList().iterator();
        while (i.hasNext()) {
        	Entity entity = (Entity) i.next();
        	textEdit.append(entity.getName() + "\n");
        }
    }

}
