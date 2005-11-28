/* PaletteWidget.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "PaletteWidget.h"
#include "MainWindow.h"
#include <qstring.h>
#include <iostream>
#include <qsettings.h>

extern QSettings globalSettings;


PaletteWidget::PaletteWidget(PaletteManager* pman,MainWindow * mw,QString name):
	QDockWindow(InDock,0,name.latin1()),_pMan(pman),_mainWindow(mw),_name(name),_firstMove(false)
{
    setResizeEnabled( TRUE );
    //setMovingEnabled( FALSE );
    setOpaqueMoving( TRUE );
    setCloseMode( QDockWindow::Always );
    //QAccel *a = new QAccel( this );
    //a->connectItem( a->insertItem( ALT + Key_B ), edit, SLOT( setFocus() ) );
    //???addToolBar( dw, Qt::DockRight );
    setCaption( tr( name.latin1() ) );
    setFixedExtentHeight( 100 );
    QObject::connect(this, SIGNAL(placeChanged(QDockWindow::Place)), this,SLOT(myPlaceChanged(QDockWindow::Place)));

  
}

void PaletteWidget::myPlaceChanged(QDockWindow::Place p)
{
	if(p==InDock)
	undock();
}

PaletteWidget::~PaletteWidget()
{}

void PaletteWidget::saveGeometry()
{
 int gx = this->x();
 int gy = this->y();
 int gwidth = this->width();
 int gheight = this->height();

 globalSettings.writeEntry(QString("/drone/palettes/")+_name+QString("/xpos"),gx);
 globalSettings.writeEntry(QString("/drone/palettes/")+_name+QString("/ypos"),gy);
 globalSettings.writeEntry(QString("/drone/palettes/")+_name+QString("/width"),gwidth);
 globalSettings.writeEntry(QString("/drone/palettes/")+_name+QString("/height"),gheight);
}

void PaletteWidget::loadGeometry()
{
  int gx = globalSettings.readEntry( QString("/drone/palettes/")+_name+QString("/xpos") ).toInt();
  int gy = globalSettings.readEntry( QString("/drone/palettes/")+_name+QString("/ypos") ).toInt();
  int gwidth = globalSettings.readEntry( QString("/drone/palettes/")+_name+QString("/width") ).toInt();
  int gheight= globalSettings.readEntry( QString("/drone/palettes/")+_name+QString("/height") ).toInt();
  setGeometry(QRect(gx,gy,gwidth,gheight));
}

void PaletteWidget::resizeEvent(QResizeEvent*e)
{
 if(_firstMove)
  saveGeometry();
 QDockWindow::resizeEvent(e);
}

void PaletteWidget::moveEvent(QMoveEvent*e)
{
 if(_firstMove)
  saveGeometry();
 QDockWindow::moveEvent(e);
}

void PaletteWidget::showEvent(QShowEvent*e)
{
 loadGeometry();
 _firstMove=true;
 QDockWindow::showEvent(e);
}

