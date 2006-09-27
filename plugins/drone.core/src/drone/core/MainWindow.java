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


import com.trolltech.qt.gui.*;

public class MainWindow extends QMainWindow {
	
    public MainWindow() throws Exception {
        setWindowTitle(tr("Drone"));
        QDockWidget dock = new QDockWidget(this);
        QTextEdit textEdit = new QTextEdit(dock);
        dock.setWidget(textEdit);
        
        addDockWidget(QDockWidget.AllDockWidgetFeatures, dock);
        throw new Exception("ttest");
        
    }

}
