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

