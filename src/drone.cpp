/* drone.cpp
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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
//#include <qsplashscreen.h>
#include "Timing.h"
#include "splash.xpm"

#include "MainWindow.h"

#include <iostream>
#include <stdio.h>

#include "Utils.h"
#include <qsettings.h>

QSettings globalSettings;

int main(int argc, char** argv)
{

  QApplication qtApp(argc, argv);
  QApplication::setGlobalMouseTracking(TRUE);

  //globalSettings.setPath("drone", "drone");
  
  
  //QSplashScreen splash(splash_xpm);
  //splash.show();

  MainWindow mainWindow;
  qtApp.setMainWidget(&mainWindow);

  //temp just because it's too fast for now
  //Timing::sleep(500);
  //

  mainWindow.show();
  
  if(argc>1)
  {
    if(argc==2)
      mainWindow.load(argv[1]);
    else 
      ERROR("Usage : drone [schema.drn]");
  }

  //splash.finish(&mainWindow);

  paint_funcs_setup();

  qtApp.exec();

  paint_funcs_free();

}

