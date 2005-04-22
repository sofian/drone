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
#include "MetaGearMaker.h"
#include "GearPropertiesDialog.h"
#include "Gear.h"
#include "MetaGear.h"
#include "GearListMenu.h"
#include "MetaGearListMenu.h"

#include "MetaGearEditor.h"
#include "PanelScrollView.h"
#include "ControlPanel.h"

#include <qmainwindow.h>
#include <qfiledialog.h>

#include <qcursor.h>
#include <qlayout.h>

#include <iostream>
#include <qpainter.h>
#include <qwmatrix.h>


const std::string SchemaEditor::NAME = "SchemaEditor";
const double SchemaEditor::ZOOM_FACTOR = 0.1;

SchemaEditor::SchemaEditor(QWidget *parent, SchemaGui *schemaGui, Engine * engine, PanelScrollView *panelScrollView) :
  QCanvasView(schemaGui, parent, NAME.c_str(),0),
  _engine(engine),
  _schemaGui(schemaGui),
  _state(IDLE),
  _movingGear(NULL),
  _zoom(1),
  _activeConnection(0),
  _contextMenuPos(0,0),
  _contextGear(NULL),
  _panelScrollView(panelScrollView),
  _selectBox(NULL)

{
  viewport()->setMouseTracking(TRUE);

  _contextMenu = new QPopupMenu(this);
  _gearListMenu = new GearListMenu(this);    
  _gearListMenu->create();
  
  _contextMenu->insertItem("Gears", _gearListMenu);
  QObject::connect(_gearListMenu, SIGNAL(gearSelected(QString)), this, SLOT(slotMenuGearSelected(QString)));
  
  _metaGearListMenu = new MetaGearListMenu(this);    
  _metaGearListMenu->create();
  
  _contextMenu->insertItem("MetaGears", _metaGearListMenu);
  QObject::connect(_metaGearListMenu, SIGNAL(metaGearSelected(QFileInfo*)), this, SLOT(slotMenuMetaGearSelected(QFileInfo*)));

  _contextMenu->insertItem("New MetaGear", this, SLOT(slotNewMetaGear()));
    
  _gearContextMenu = new QPopupMenu(this);
  _gearContextMenu->insertItem("delete",  this, SLOT(slotGearDelete()));
  _gearContextMenu->insertItem("Properties", this, SLOT(slotGearProperties()));
  _gearContextMenu->insertItem("About");    
  
  _metaGearContextMenu = new QPopupMenu(this);
  _metaGearContextMenu->insertItem("delete", this, SLOT(slotGearDelete()),Key_Delete);
  _metaGearContextMenu->insertItem("Select All", this, SLOT(slotGearSelectAll()),CTRL + Key_A);
  _metaGearContextMenu->insertItem("Copy", this, SLOT(slotGearCopy()),CTRL + Key_C);
  _metaGearContextMenu->insertItem("Paste", this, SLOT(slotGearPaste()),CTRL + Key_V);
  _metaGearContextMenu->insertItem("Properties", this, SLOT(slotGearProperties()));  
  _metaGearContextMenu->insertItem("About");    
  _metaGearContextMenu->insertSeparator();
  _metaGearContextMenu->insertItem("Save MetaGear",  this, SLOT(slotSaveMetaGear()));


  // plug context menu initialization
  _plugContextMenu = new QPopupMenu(this);
  _plugContextMenu->insertItem("expose", this, SLOT(slotPlugExpose()),0,EXPOSE);
  _plugContextMenu->insertItem("unexpose", this, SLOT(slotPlugUnexpose()),0,UNEXPOSE);

}

SchemaEditor::~SchemaEditor()
{

}

void SchemaEditor::keyPressEvent(QKeyEvent *e)
{     
  QCanvasItemList l=canvas()->allItems();

  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
  {
    if ( (*it)->rtti() == GearGui::CANVAS_RTTI_GEAR)
      if(((GearGui*)(*it))->keyEvent(e))
        e->accept();
  }
  std::cerr<<"Keypress:"<<e->ascii()<<std::endl;
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


void SchemaEditor::contentsMousePressEvent(QMouseEvent* mouseEvent)
{
  QPoint p = inverseWorldMatrix().map(mouseEvent->pos());
  GearGui *gearGui = _schemaGui->testForGearCollision(p);
  PlugBox *selectedPlugBox;
    
  
  if (gearGui!=NULL)
  {    
    //send mouse events
    gearGui->mouseEvent(p, mouseEvent->button());


    if (gearGui->titleBarHitted(p))
    {
      // select only the clicked gear
      if(mouseEvent->state()&Qt::ControlButton || mouseEvent->state()&Qt::ShiftButton)
        toggleGearSelection(gearGui);
      else if(!gearGui->isSelected())
        selectOneGear(gearGui);
    }

    //on left button we...
    if (mouseEvent->button() == Qt::LeftButton)
    {      
      //plug collision to start connections ?
      if ( ((selectedPlugBox = gearGui->plugHitted(p)) != NULL))
      {        
        _state=CONNECTING;
        _activeConnection = new ConnectionItem(canvas());
        _activeConnection->setSourcePlugBox(selectedPlugBox);
        _activeConnection->show();        

      } else if (gearGui->titleBarHitted(p))
      {
        // start moving it (but only if it has not been unselected, in
        // which case it is weird to move remaining gears.. (well, in my opinion JK)
        if(gearGui->isSelected())
        {
          _state = MOVING_GEAR;            
          _movingGear = gearGui;
          _movingGearStartPos = p;            
        }
      }
    }
  } 
  else if (mouseEvent->button() == Qt::LeftButton)
  {
    delete _selectBox;
    _selectBox=NULL;
    _movingGear=NULL;
    unselectAllGears();
    _state = DRAGGING_SELECT_BOX;
    _selectBoxStartPos = p;
  }

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
  PlugBox *selectedPlugBox;

  QPoint p = inverseWorldMatrix().map(mouseEvent->pos());
  
  switch (_state)
  {
  case IDLE:

    //connection hilighting
    _schemaGui->unHilightAllConnections();
    connectionItem = _schemaGui->testForConnectionCollision(p);
    if (connectionItem!=NULL)
    {
      connectionItem->hiLight(true);
    } 

    //connection plugboxes hilighting
    gearGui = _schemaGui->testForGearCollision(p);   
    if (gearGui!=NULL)
    {
      gearGui->mouseEvent(p, mouseEvent->state());
      gearGui->performPlugHighligthing(p);
    }

    break;

  case MOVING_GEAR:
    if (_movingGear!=NULL)
    {
      moveSelectedGearsBy(p.x() -_movingGearStartPos.x(), p.y() - _movingGearStartPos.y());
//    _schemaGui->moveGearBy(_movingGear, p.x() -_movingGearStartPos.x(), p.y() - _movingGearStartPos.y());
      _movingGearStartPos = p;        
    }
    break;

  case CONNECTING:    
    _activeConnection->setConnectionLineEndPoint(p);

    gearGui = _schemaGui->testForGearCollision(p);   

    if (gearGui!=NULL)
    {
      if ( ((selectedPlugBox = gearGui->plugHitted(p)) != 0))
      {      
        if (_activeConnection->sourcePlugBox()->canConnectWith(selectedPlugBox))
            gearGui->performPlugHighligthing(selectedPlugBox);
        else
            gearGui->unHilightAllPlugBoxes();
      }
      else
        gearGui->unHilightAllPlugBoxes();
    }

    break;           

  case DRAGGING_SELECT_BOX:
    //delete _selectBox;
    if(!_selectBox)
      _selectBox = new QCanvasRectangle(QRect(_selectBoxStartPos, p),_schemaGui);
    else 
      _selectBox->setSize(p.x()-_selectBoxStartPos.x(),p.y()-_selectBoxStartPos.y());
    _selectBox->setSelected(!_selectBox->isSelected());
    _selectBox->show();
    //_schemaGui->setUpdatePeriod(100);
    _schemaGui->update();
    selectGearsInRectangle(_selectBox->boundingRect());
    break;

  }
}

void SchemaEditor::contentsMouseReleaseEvent(QMouseEvent *mouseEvent)
{
  QPoint p = inverseWorldMatrix().map(mouseEvent->pos());
  GearGui *gearGui = _schemaGui->testForGearCollision(p);
  PlugBox *selectedPlugBox=NULL;

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
      if ( ((selectedPlugBox = gearGui->plugHitted(p)) != 0))             
        if (_activeConnection->sourcePlugBox()->canConnectWith(selectedPlugBox))
          _schemaGui->connect(_activeConnection->sourcePlugBox(), selectedPlugBox);
    }

    //delete our temporary connectionItem
    delete _activeConnection;
    
    canvas()->update();
    
    _state=IDLE;
    break;
  case DRAGGING_SELECT_BOX:
    delete _selectBox;
    _selectBox=NULL;
    _state=IDLE;
    _schemaGui->update();
    break;

  }
}

void SchemaEditor::contentsMouseDoubleClickEvent(QMouseEvent *mouseEvent)
{    
  ConnectionItem *connectionItem;
  GearGui *gearGui;

  QPoint p = inverseWorldMatrix().map(mouseEvent->pos());
  gearGui = _schemaGui->testForGearCollision(p);

  switch (_state)
  {
  case IDLE:
  {  
    _schemaGui->unHilightAllConnections();
    connectionItem = _schemaGui->testForConnectionCollision(p);

    if (connectionItem!=NULL)
      _schemaGui->disconnect(connectionItem->sourcePlugBox(), connectionItem->destPlugBox());      
    
    //handle double-click on metagear
    if (gearGui!=NULL && gearGui->gear()->isMeta())
    {
      QDialog metaGearEditorDialog(this);  
      QVBoxLayout layout(&metaGearEditorDialog, 1);
      metaGearEditorDialog.setCaption(gearGui->gear()->name().c_str());
      metaGearEditorDialog.resize(640,480);
      MetaGearEditor metaGearEditor(&metaGearEditorDialog, (MetaGear*)gearGui->gear(), _engine);
      layout.addWidget(&metaGearEditor);
      metaGearEditorDialog.exec();
      
      //todo : temp...
      ((MetaGear*)(gearGui->gear()))->createPlugs();
    }

    if (gearGui!=NULL && gearGui->titleBarHitted(p))
        selectOneGear(gearGui);
    
    break;
  }  
      
  default:
    break;
  }
}

void SchemaEditor::contextMenuEvent(QContextMenuEvent *contextMenuEvent)
{    
  QPoint p = inverseWorldMatrix().map(contextMenuEvent->pos());
  PlugBox *selectedPlugBox;

  GearGui *gearGui = _schemaGui->testForGearCollision(p);

  if (gearGui!=NULL)
  {
    _contextGear = gearGui;
    if(((selectedPlugBox = gearGui->plugHitted(p)) != 0))
    {
      if(selectedPlugBox->plug()->exposed())
      {
        _plugContextMenu->setItemVisible(EXPOSE, false);
        _plugContextMenu->setItemVisible(UNEXPOSE, true);
      } else
      {
        _plugContextMenu->setItemVisible(EXPOSE, true);
        _plugContextMenu->setItemVisible(UNEXPOSE, false);
      }
      _contextPlug = selectedPlugBox;
      _plugContextMenu->popup(QCursor::pos());
      
    } else
    {
      if (_contextGear->gear()->isMeta())
      {
        _metaGearContextMenu->popup(QCursor::pos());
      }
      else
        _gearContextMenu->popup(QCursor::pos());
    }
  } else
  {
    _contextGear = NULL; 
    _contextMenuPos = p;
    _contextMenu->popup(QCursor::pos());
  }


  QCanvasView::contextMenuEvent(contextMenuEvent);
}

void SchemaEditor::slotMenuGearSelected(QString name)
{        
  addGear(name.ascii(), _contextMenuPos.x(), _contextMenuPos.y());    
}

void SchemaEditor::slotMenuMetaGearSelected(QFileInfo* metaGearFileInfo)
{  
  addMetaGear(metaGearFileInfo->filePath().ascii(), _contextMenuPos.x(), _contextMenuPos.y());    
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
  deleteSelectedGears();
  _contextGear = NULL;
}

/**
 * React on Expose selection in the plug context menu
 */
void SchemaEditor::slotPlugExpose()
{
  _contextPlug->plug()->exposed(true);
} 

/**
 * React on Unexpose selection in the plug context menu
 */
void SchemaEditor::slotPlugUnexpose()
{
  _contextPlug->plug()->exposed(false);
} 

void SchemaEditor::slotNewMetaGear()
{
  addNewMetaGear(_contextMenuPos.x(), _contextMenuPos.y());
}

void SchemaEditor::slotSaveMetaGear()
{
  if (!_contextGear->gear()->isMeta())
  {
    std::cout << "not a metagear, cannot save!!!" << std::endl; 
    return;
  }
   
  MetaGear* metaGear = (MetaGear*)_contextGear->gear();
  
  //mangle suggested filename
  std::string suggestedFilename=metaGear->fullPath();  
  if (suggestedFilename.empty())
    suggestedFilename=MetaGearMaker::METAGEAR_PATH + "/" + metaGear->name();
  

  std::string filename = QFileDialog::getSaveFileName(suggestedFilename, "*" + MetaGear::EXTENSION + ";;" + "*.*", 
                                                      this, "Save as", "Save as");
  
  if (!filename.empty())
  {
    //set the extension if not already present
    if (filename.find(MetaGear::EXTENSION.c_str(), filename.size()-MetaGear::EXTENSION.size())==std::string::npos)
      filename.append(MetaGear::EXTENSION);  
    
    metaGear->save(filename);    
    
    //rename the metagear to the new saved name
    QFileInfo fileInfo(filename);
    _schemaGui->renameGear(_contextGear, fileInfo.baseName());
    
  }

}

void SchemaEditor::addGear(std::string name, int posX, int posY)
{
  _schemaGui->addGear(name, posX, posY);    
}

void SchemaEditor::addMetaGear(std::string filename, int posX, int posY)
{  
  MetaGear *metaGear = _schemaGui->addMetaGear(filename, posX, posY);
  associateControlPanelWithMetaGear(metaGear);
}

void SchemaEditor::addNewMetaGear(int posX, int posY)
{
  MetaGear *metaGear = _schemaGui->newMetaGear(posX, posY);
  
  associateControlPanelWithMetaGear(metaGear);
}

void SchemaEditor::deleteSelectedGears()
{
  std::vector<GearGui*> allGears = _schemaGui->getSelectedGears();
  for(unsigned int i=0;i<allGears.size();++i)
    _schemaGui->removeGear(allGears[i]);
}

void SchemaEditor::unselectAllGears()
{
  std::vector<GearGui*> allGears = _schemaGui->getSelectedGears();
  for(unsigned int i=0;i<allGears.size();++i)
    allGears[i]->setSelected(false);
  _schemaGui->update();
}

void SchemaEditor::selectAllGears()
{
  std::vector<GearGui*> allGears = _schemaGui->getAllGears();
  for(unsigned int i=0;i<allGears.size();++i)
    allGears[i]->setSelected(true);
  _schemaGui->update();
}

void SchemaEditor::selectGearsInRectangle(QRect rect)
{
  std::vector<GearGui*> allGears = _schemaGui->getAllGears();
  for(unsigned int i=0;i<allGears.size();++i)
    allGears[i]->setSelected(allGears[i]->boundingRect().intersects(rect));
  _schemaGui->update();
}

void SchemaEditor::selectOneGear(GearGui* gear)
{
  std::vector<GearGui*> allGears = _schemaGui->getAllGears();
  for(unsigned int i=0;i<allGears.size();++i)
    allGears[i]->setSelected(allGears[i]==gear);
  _schemaGui->update();
}

void SchemaEditor::moveSelectedGearsBy(int x, int y)
{
  std::vector<GearGui*> allGears = _schemaGui->getSelectedGears();
  for(unsigned int i=0;i<allGears.size();++i)
    _schemaGui->moveGearBy( allGears[i],x,y);
  _schemaGui->update();
}

void SchemaEditor::toggleGearSelection(GearGui* gear)
{
  gear->toggleSelection();
  _schemaGui->update();
}

void SchemaEditor::associateControlPanelWithMetaGear(MetaGear *metaGear)
{
  //create and associate a control panel with this metagear
  //_panelScrollView->addControlPanel(metaGear);    
}

void SchemaEditor::slotGearCopy()
{
  QDomDocument doc("Clipboard");
  
  QDomElement clipboardElem = doc.createElement("Clipboard");
  doc.appendChild(clipboardElem);

  if(!_schemaGui->save(doc, clipboardElem,true))
    return;

  //save to file  
  QString str;
  QTextStream stream(str,IO_WriteOnly);
  doc.save(stream,4);
  _engine->setClipboardText(str.latin1());
  std::cerr<<_engine->getClipboardText()<<std::endl;
  
}

void SchemaEditor::slotGearPaste()
{
  unselectAllGears();
  QDomDocument doc("Clipboard");

  QString str(_engine->getClipboardText());

  QString errMsg;
  int errLine;
  int errColumn;
  if (!doc.setContent(str, true, &errMsg, &errLine, &errColumn))
  {
    std::cout << "parsing error in clipboard"<< std::endl;
    std::cout << errMsg.ascii() << std::endl;
    std::cout << "Line: " <<  errLine << std::endl;
    std::cout << "Col: " <<  errColumn << std::endl;
    return;
  }
  
  QDomNode clipboardNode = doc.firstChild();
  QDomNode schemaNode = clipboardNode.firstChild();
  
  if (schemaNode.isNull())
  {
    std::cout << "Bad drone project, main schema node isNull" << std::endl;
    return;
  }
  
  QDomElement schemaElem = schemaNode.toElement();
  
  if (schemaElem.isNull())
  {
    std::cout << "Bad drone project, main schema elem isNull" << std::endl;
    return;
  }
  
  _schemaGui->getSchema()->load(schemaElem, true);
  _schemaGui->rebuildSchema();

}

void SchemaEditor::slotGearSelectAll()
{
  std::cerr<<"select all!!!"<<std::endl;
  selectAllGears();
}
