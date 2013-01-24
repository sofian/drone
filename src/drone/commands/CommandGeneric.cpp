/* 
 * File:   CommandGeneric.cpp
 * Author: julien
 * 
 * Created on January 21, 2013, 7:31 PM
 */

#include "CommandGeneric.h"
#include "MainWindow.h"

CommandGeneric::CommandGeneric():
_skipFirstRedo(true)
{
  _snapshotBefore = MainWindow::getInstance()->getProject()->getSnapshot(); 

}

void CommandGeneric::saveSnapshotAfter()
{
  _snapshotAfter = MainWindow::getInstance()->getProject()->getSnapshot(); 
}

CommandGeneric::~CommandGeneric()
{
}

void CommandGeneric::undo()
{
  MainWindow::getInstance()->getProject()->load(_snapshotBefore, Drone::RestoreSnapshot);
}

void CommandGeneric::redo()
{
  if(_skipFirstRedo)
  {
    _skipFirstRedo=false;
    return;
  }
  MainWindow::getInstance()->getProject()->load(_snapshotAfter, Drone::RestoreSnapshot);
}



