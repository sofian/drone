/* PaletteManager.h
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

#ifndef PaletteManager_INCLUDED
#define PaletteManager_INCLUDED

#include "qwidget.h"
#include <qstring.h>
#include <qmainwindow.h>
#include <qtimer.h>

class MainWindow;
class PaletteWidget;

class PaletteManager: public QObject
{
Q_OBJECT
  public:
  PaletteManager(MainWindow*);
  PaletteWidget* addPalette(QString str);
  virtual ~PaletteManager();
  void undockAllPalettes();
  void regroup();
  void moveGroup(QPoint p,PaletteWidget*);
  int numPalettes(){return _pw.size();}
  bool movingGroup(){return _movingGroup;}
public slots:
  void updateSizes();
protected:
  QTimer* _timer;
  std::list<PaletteWidget*> _pw; 
  MainWindow* _mainWindow;
  bool _movingGroup;
  
};

#endif
