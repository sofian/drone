

#include <qapplication.h>
//#include <qsplashscreen.h>
#include "Timing.h"
#include "splash.xpm"

#include "MainWindow.h"

#include <iostream>
#include <stdio.h>
    
int main(int argc, char** argv)
{

    QApplication qtApp(argc, argv);
    QApplication::setGlobalMouseTracking(TRUE);
    
    //QSplashScreen splash(splash_xpm);
    //splash.show();

    MainWindow mainWindow;
    qtApp.setMainWidget(&mainWindow);

    //temp just because it's too fast for now
//    Timing::sleep(500);
    //

    mainWindow.show();

    //splash.finish(&mainWindow);
    

    qtApp.exec();

}

