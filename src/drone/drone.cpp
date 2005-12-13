/* drone.cpp
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

#include <qapplication.h>
#include <qsplashscreen.h>
#include "Timing.h"
#include "DroneCore.h"
#include "splash.xpm"

#include "MainWindow.h"

#include <iostream>
#include <stdio.h>
#include <qdialog.h>
#include <qsettings.h>

QSettings globalSettings;


int main(int argc, char** argv)
{
  QApplication qtApp(argc, argv);
  QApplication::setGlobalMouseTracking(TRUE);
      
  QSplashScreen splash(splash_xpm);
  splash.show();

  //init the drone core
	DroneCore::init();
	
  Engine engine(0);

  MainWindow mainWindow(&engine);
  mainWindow.adjustSize();
  qtApp.setMainWidget(&mainWindow);

  splash.hide();
  mainWindow.show();
  mainWindow.undockPaletteHack();
  if(argc>1)
  {
    if(argc==2)
      mainWindow.load(argv[1]);
    else 
      error("Usage : drone [schema.drn]");
  }


  qtApp.exec();
  //release the core
	DroneCore::release();
  
}

