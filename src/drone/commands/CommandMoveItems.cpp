/* 
 * File:   CommandMoveItems.cpp
 * Author: julien
 * 
 * Created on January 9, 2013, 2:25 PM
 */

#include "CommandMoveItems.h"
#include "MainWindow.h"
#include "SchemaGui.h"
#include <QDebug>
#include <QGraphicsItem>

CommandMoveItems::CommandMoveItems(QList<QString> &itemList,QPointF delta):
  _itemList(itemList),
  _delta(delta),  
  _skipFirstRedo(true)
{
  qDebug()<<"created an commandMoveItem with"<<itemList.count()<<" elements";
}

CommandMoveItems::~CommandMoveItems()
{
}

void CommandMoveItems::undo()
{
  qDebug()<<"undo! "<<_itemList<<" have been moved back "<<_delta;
  SchemaGui* schemaGui(MainWindow::getInstance()->getSchemaGui());
  schemaGui->moveItemsBy(schemaGui->getItemsByUUID(_itemList),QPointF(-_delta.x(),-_delta.y()));
}

void CommandMoveItems::redo()
{
  // skip first call to redo because the first movement operation 
  // has been made by QGraphicsScene
  if(_skipFirstRedo)
  {
    _skipFirstRedo=false;
    return;
  }
  qDebug()<<"redo! "<<_itemList<<" have been moved again "<<_delta;
  SchemaGui* schemaGui(MainWindow::getInstance()->getSchemaGui());
  schemaGui->moveItemsBy(schemaGui->getItemsByUUID(_itemList),_delta);
}


