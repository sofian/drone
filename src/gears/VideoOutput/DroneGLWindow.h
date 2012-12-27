//
//  DroneGLWindow.h
//  drone
//
//  Created by Mathieu Guindon on 2012-12-23.
//  Copyright (c) 2012 drone. All rights reserved.
//

#ifndef __drone__DroneGLWindow__
#define __drone__DroneGLWindow__

#include <qdialog.h>

//we need to create our own widget for the window containing the droneqglwidget
//to change the behavior of the close event to do nothing
//since we dont want this window to be closed by the user
class DroneGLWindow : public QDialog
{
public:
  DroneGLWindow(QWidget* parent) : QDialog(parent)
  {
    setModal(false);
    resize(320,240);
  }
protected:
  void closeEvent(QCloseEvent *){}
};

#endif /* defined(__drone__DroneGLWindow__) */
