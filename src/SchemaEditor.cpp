/* SchemaEditor.cpp
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

#include "SchemaEditor.h"
#include "GearGui.h"
#include "ConnectionItem.h"
#include "PlugBox.h"
#include "Engine.h"
#include "GearMaker.h"
#include "GearPropertiesDialog.h"
#include "Gear.h"

#include <qmainwindow.h>

#include <qcursor.h>

#include <iostream>
#include <qpainter.h>
#include <qwmatrix.h>


const std::string SchemaEditor::NAME = "SchemaEditor";
const double SchemaEditor::ZOOM_FACTOR = 0.1;

SchemaEditor::SchemaEditor(QWidget *parent, SchemaGui *schemaGui, Engine * engine) :
  QCanvasView(schemaGui, parent, NAME.c_str(),0),
  _engine(engine),
  _schemaGui(schemaGui),
  _state(IDLE),
  _movingGear(NULL),
  _zoom(1),
  _activeConnection(NULL),
  _allGearsMenuPos(0,0),
  _contextGear(NULL)
{
  viewport()->setMouseTracking(TRUE);

  GearMaker::getAllGearsName(_allGearsName);


  _allGearsMenu = new QPopupMenu(this);    
  _allGearsMenu->insertItem("Gears", createGearsMenu());

  _gearMenu = new QPopupMenu(this);
  _gearMenu->insertItem("delete",  this, SLOT(slotGearDelete()));
  _gearMenu->insertItem("Properties", this, SLOT(slotGearProperties()));
  _gearMenu->insertItem("About");    


}

SchemaEditor::~SchemaEditor()
{

}

void SchemaEditor::keyPressEvent(QKeyEvent *e)
{     
  QCanvasItemList l=canvas()->allItems();

  std::cerr<<"!!!!!!!!!!!!!!"<<e->ascii()<<" "<<e->stateAfter()<<std::endl;

  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
  {
    if ( (*it)->rtti() == GearGui::CANVAS_RTTI_GEAR)
      if(((GearGui*)(*it))->keyEvent(e))
        e->accept();
  }
}

void SchemaEditor::keyReleaseEvent(QKeyEvent *e)
{     
  QCanvasItemList l=canvas()->allItems();

  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
  {
    if ( (*it)->rtti() == GearGui::CANVAS_RTTI_GEAR)
      if(((GearGui*)(*it))->keyEvent(e))
        e->accept();
  }
}

GearGui* SchemaEditor::testForGearCollision(const QPoint &p)
{     
  QCanvasItemList l=canvas()->collisions(p);

  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
  {
    if ( (*it)->rtti() == GearGui::CANVAS_RTTI_GEAR)
    {
      return(GearGui*)(*it);
    }
  }
  return NULL;
}

ConnectionItem* SchemaEditor::testForConnectionCollision(const QPoint &p)
{        
  QCanvasItemList l=canvas()->collisions(p);

  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
  {
    if ( (*it)->rtti() == ConnectionItem::CANVAS_RTTI_CONNECTION)
      return(ConnectionItem*) (*it);
  }
  return NULL;
}

void SchemaEditor::unHilightAllConnections()
{        
  QCanvasItemList l=canvas()->allItems();
  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
  {
    if ( (*it)->rtti() == ConnectionItem::CANVAS_RTTI_CONNECTION)
    {
      ((ConnectionItem*)(*it))->hiLight(false);
    }
  }    
}

void SchemaEditor::contentsMousePressEvent(QMouseEvent* mouseEvent)
{
  QPoint p = inverseWorldMatrix().map(mouseEvent->pos());
  GearGui *gearGui = testForGearCollision(p);
  PlugBox *selectedPlugBox;
    
  /*
  QMainWindow *wnd = new QMainWindow();  
  SchemaGui *tschemaGui = new SchemaGui(_engine->mainSchema(), _engine);
  SchemaEditor *schemaEditor = new SchemaEditor(wnd, tschemaGui, _engine);
  wnd->setCentralWidget(schemaEditor); 
  wnd->show();
  */

  if (gearGui!=NULL)
  {    
    //send mouse events
    gearGui->mouseEvent(p, mouseEvent->button());

    //on left button we...
    if (mouseEvent->button() == Qt::LeftButton)
    {      
      //plug collision to start connections ?
      if ( ((selectedPlugBox = gearGui->plugHitted(p)) != NULL))
      {        
        _state=CONNECTING;
        _activeConnection = new ConnectionItem(canvas());
        _activeConnection->setStartingPlugBox(selectedPlugBox);
        _activeConnection->show();        

      } else if (gearGui->titleBarHitted(p))
      {
        _activeConnection = NULL;
        _state = MOVING_GEAR;            
        _movingGear = gearGui;
        _movingGearStartPos = p;            
      }
    }
  } else
    _movingGear=NULL;

}

void SchemaEditor::contentsWheelEvent(QWheelEvent * wheelEvent)
{
  if (wheelEvent->delta() > 0)
    zoom(ZOOM_FACTOR);
  else
    zoom(-ZOOM_FACTOR);
}

void SchemaEditor::zoomIn()
{
  zoom(ZOOM_FACTOR);
}

void SchemaEditor::zoomOut()
{
  zoom(-ZOOM_FACTOR);
}

void SchemaEditor::zoom(float factor)
{
  _zoom+=factor;
  if (_zoom < 0.2f)
    _zoom = 0.2f;

  int oldcenterx = visibleWidth()/2;
  int oldcentery = visibleHeight()/2;
  int x, y;
  viewportToContents(oldcenterx, oldcentery, x, y);
  inverseWorldMatrix().map(x, y, &x, &y);
	
  QWMatrix wm;
  wm.scale(_zoom, _zoom);
  setWorldMatrix(wm);

  
  //worldMatrix().map(x, y, &x, &y);
  //setContentsPos(x-oldcenterx, y-oldcentery);

}

void SchemaEditor::contentsMouseMoveEvent(QMouseEvent *mouseEvent)
{
  GearGui *gearGui;
  ConnectionItem *connectionItem;

  QPoint p = inverseWorldMatrix().map(mouseEvent->pos());
  
  switch (_state)
  {
  case IDLE:

    unHilightAllConnections();
    connectionItem = testForConnectionCollision(p);

    if (connectionItem!=NULL)
    {
      connectionItem->hiLight(true);
    } else
    {
      //if no connection hitted test for gear collision
      gearGui = testForGearCollision(p);   

      if (gearGui!=NULL)
      {
        gearGui->mouseEvent(p, mouseEvent->state());
        gearGui->performPlugHighligthing(p, NULL);
      }

    }

    break;

  case MOVING_GEAR:
    if (_movingGear!=NULL)
    {
      moveGearBy(_movingGear, p.x() -_movingGearStartPos.x(), p.y() - _movingGearStartPos.y());
      _movingGearStartPos = p;        
    }
    break;

  case CONNECTING:
    _activeConnection->setConnectionLineEndPoint(p);

    gearGui = testForGearCollision(p);   

    if (gearGui!=NULL)
    {
      gearGui->performPlugHighligthing(p, _activeConnection->sourcePlugBox());
    }


    break;            
  }
}

void SchemaEditor::contentsMouseReleaseEvent(QMouseEvent *mouseEvent)
{
  QPoint p = inverseWorldMatrix().map(mouseEvent->pos());
  GearGui *gearGui = testForGearCollision(p);

  if (gearGui!=NULL)
  {
    //send mouse events
    gearGui->mouseEvent(p, mouseEvent->stateAfter());
  }
  switch (_state)
  {
  case IDLE:
    break;
  case MOVING_GEAR:
    _state=IDLE;
    break;
  case CONNECTING:
  
    if (gearGui!=NULL)
    {
      if (!_activeConnection->createConnection(gearGui->plugHitted(p)))
        delete _activeConnection;
    } else
      delete _activeConnection;

    canvas()->update();
    _state=IDLE;
    break;
  }
}

void SchemaEditor::contentsMouseDoubleClickEvent(QMouseEvent *mouseEvent)
{    
  ConnectionItem *connectionItem;
  GearGui *gearGui;

  QPoint p = inverseWorldMatrix().map(mouseEvent->pos());

  switch (_state)
  {
  case IDLE:

    unHilightAllConnections();
    connectionItem = testForConnectionCollision(p);

    if (connectionItem!=NULL)
    {
      delete connectionItem;
      canvas()->update();
    }

    gearGui = testForGearCollision(p);   

    if (gearGui!=NULL)
    {
      if(!gearGui->mouseEvent(p, mouseEvent->button()))
        slotGearProperties();
    }
    break;
  default:
    break;
  }
}

void SchemaEditor::moveGearBy(GearGui *gearGui, int x, int y)
{
  gearGui->moveBy(x, y); 
  canvas()->update();    
}

QPopupMenu* SchemaEditor::createGearsMenu()
{
  QPopupMenu *gearsMenu = new QPopupMenu(this);    
  QObject::connect(gearsMenu, SIGNAL(activated(int)), this, SLOT(slotMenuItemSelected(int)));

  int i=0;
  for (std::vector<std::string>::iterator it = _allGearsName.begin(); it != _allGearsName.end(); ++it, ++i)
    gearsMenu->insertItem((*it), i);

  return gearsMenu;
}

void SchemaEditor::contextMenuEvent(QContextMenuEvent *contextMenuEvent)
{    
  QPoint p = inverseWorldMatrix().map(contextMenuEvent->pos());
  

  GearGui *gearGui = testForGearCollision(p);

  if (gearGui!=NULL)
  {
    _contextGear = gearGui;
    _gearMenu->popup(QCursor::pos());
  } else
  {
    _contextGear = NULL;
    _allGearsMenuPos = p;
    _allGearsMenu->popup(QCursor::pos());
  }


  QCanvasView::contextMenuEvent(contextMenuEvent);
}

void SchemaEditor::slotMenuItemSelected(int id)
{
  std::cout << id << std::endl;
  //only process for gears menuitem
  if (id < 0)
    return;

  if ((unsigned int)id > _allGearsName.size()-1)
    return;

  _schemaGui->addGear(_allGearsName[id], _allGearsMenuPos.x(), _allGearsMenuPos.y());  
}

void SchemaEditor::slotGearProperties()
{
  if (_contextGear == NULL)
    return;

  GearPropertiesDialog *gearPropertiesDialog;

  gearPropertiesDialog = new GearPropertiesDialog(this, _contextGear->gear(), _engine);
  gearPropertiesDialog->exec();
  delete gearPropertiesDialog;

  _contextGear=NULL;

}

void SchemaEditor::slotGearDelete()
{
  if (_contextGear == NULL)
    return;

  _schemaGui->removeGear(_contextGear);
}


