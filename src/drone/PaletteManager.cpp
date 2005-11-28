/* PaletteManager.cpp
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

#include "PaletteManager.h"
#include "PaletteWidget.h"
#include "MainWindow.h"
#include <qsettings.h>
#include <iostream>


PaletteManager::PaletteManager(MainWindow *mainWindow):_mainWindow(mainWindow)
{
  
}

PaletteManager::~PaletteManager()
{
}

PaletteWidget* PaletteManager::addPalette(QString name)
{
  PaletteWidget* pwidg = new PaletteWidget(this, _mainWindow,name);
  _pw.push_back(pwidg);

  return pwidg;

}

void PaletteManager::undockAllPalettes()
{
  for(std::list<PaletteWidget*>::iterator it = _pw.begin();it!=_pw.end();++it)
      (*it)->undock();
}
