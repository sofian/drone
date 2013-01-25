/* SchemaGui.cpp
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


#include "SchemaGui.h"

#include "Schema.h"
#include "Gear.h"
#include "Control.h"
#include "GearControl.h"
#include "MetaGear.h"
#include "gearFactory/GearMaker.h"
#include "GearGui.h"
#include "PlugBox.h"
#include "commands/CommandGeneric.h"
#include "ConnectionItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QVarLengthArray>
#include <Q3FileDialog>
#include <QTextStream>
#include "MainWindow.h"

const int SchemaGui::DEFAULT_CANVAS_SIZE_X = 2048;
const int SchemaGui::DEFAULT_CANVAS_SIZE_Y = 2048;


SchemaGui::SchemaGui(Schema *schema, Engine *engine) :
  QGraphicsScene(0,0,DEFAULT_CANVAS_SIZE_X, DEFAULT_CANVAS_SIZE_Y),
  _activeConnection(NULL),
  _connecting(false),
  _pasteOffset(),
  _engine(engine),
  _maxZValue(0),
  _moving(No),
  _selectionChangeNotificationBypass(false),
  _movingItems(),
  _movingStartPos(0,0),
  _autoSelectNewElements(false)
{
  QObject::connect(this,SIGNAL(selectionChanged()),this,SLOT(selectionHasChanged()));
  //QObject::connect(this, SIGNAL(itemMoved(DiagramItem*,QPntF)),
  //           this, SLOT(itemMoved(DiagramItem*,QPointF)));
  setSchema(schema);
}

SchemaGui::~SchemaGui()
{
}


void SchemaGui::setSchemaEditor(SchemaEditor* se)
{
  _schemaEditor = se;
}

SchemaEditor* SchemaGui::getSchemaEditor() const
{
  return _schemaEditor;
}

void SchemaGui::setSchema(Schema *schema)
{
  clear(); 
  _schema = schema;
  QObject::connect(schema,SIGNAL(gearAdded(Schema&,Gear&)),this,SLOT(slotGearAdded(Schema&,Gear&)));
  QObject::connect(schema,SIGNAL(gearRemoved(Schema&,Gear&)),this,SLOT(slotGearRemoved(Schema&,Gear&)));
  QObject::connect(schema,SIGNAL(connectionCreated(Schema&,Connection)),this,SLOT(slotConnectionCreated(Schema&,Connection)));
  QObject::connect(schema,SIGNAL(connectionRemoved(Schema&,Connection)),this,SLOT(slotConnectionRemoved(Schema&,Connection)));
  
  //_schema->add
  rebuildSchema();
}

void SchemaGui::slotGearAdded(Schema &schema,Gear &gear)
{
  GearControl* controlGear;
  if (!gear.getGearGui())
  {
    // the GearGui constructor will take care of establishing the 
    // GearGui<->Control relation in case we have a GearControl
    GearGui* ggui = new GearGui(&gear);
    gear.setGearGui(ggui);
    addItem(ggui);
    if(_autoSelectNewElements)
      ggui->setSelected(true);
  }
}

void SchemaGui::slotGearRemoved(Schema &schema,Gear &gear)
{
  if(gear.getGearGui())
  {
    // the GearGui destructor will take care of deleting the 
    // control in case it has one
    delete gear.getGearGui();
  }
}


void SchemaGui::rebuildSchema()
{
  //add gearguis
  QList<Gear*> gears = _schema->getGears();
  
  
/*  GearGui *gearGui=NULL;
  foreach(Gear* gear,gears)
  {
		//gearGui=gear->getGearGui();
		//addItem(gearGui);
    //gearGui->show();
  }
 */
	
  //add connectionItems
  QList<Connection*> connections(_schema->getAllConnections());
  
  //PlugBox *sourcePlugBox;
  //PlugBox *destPlugBox;
  Gear *gearA;
  Gear *gearB;

  foreach(Connection* conn,connections)
  {   
    gearA = _schema->getGearByUUID(conn->gearA());
    gearB = _schema->getGearByUUID(conn->gearB());

    if (gearA && gearB)
		{
//			sourcePlugBox = gearA->getGearGui()->getOutputPlugBox(conn->output());
//			destPlugBox = gearB->getGearGui()->getInputPlugBox(conn->input());
//			sourcePlugBox->connect(destPlugBox);
		}
		
    delete conn;//free temporary Connection
  }

  update();

}



  void SchemaGui::drawBackground ( QPainter * painter, const QRectF & rect )
  {
    painter->fillRect(rect,QColor(50,50,50));
    QVarLengthArray<QLineF, 200> lines;
        for (int i = 0; i <= 100; i ++) {
          if(i%5!=0)
          {
            lines.append(QLineF(i*30+0.5,0,i*30+0.5,10000));
            lines.append(QLineF(0,i*30+0.5,10000,i*30+0.5));
          }
        }
    painter->setPen(QPen(QColor(60,60,60),0.5));
    painter->drawLines(lines.data(), lines.size());
    
    QVarLengthArray<QLineF, 40> lines2;
        for (int i = 0; i <= 20; i ++) {
            lines2.append(QLineF(i*150+0.5,0,i*150+0.5,10000));
            lines2.append(QLineF(0,i*150+0.5,10000,i*150+0.5));
        }
    painter->setPen(QPen(QColor(70,70,70),0.5));
    painter->drawLines(lines2.data(), lines2.size());
  }




  
Gear* SchemaGui::addGear(QString fullname, QPointF pos)
{
  CommandGeneric* com = new CommandGeneric();
  Gear* gear = GearMaker::instance()->makeGear(fullname);
  if (!gear)
    return NULL;
    
  GearGui *gearGui = new GearGui(gear);    
  gear->setGearGui(gearGui);
  addItem(gearGui);    
  gearGui->setPos(pos);    
  gearGui->setZValue(0);
  gearGui->update();
  
  _schema->addGear(*gear);

  com->saveSnapshotAfter();
  com->setText(QString("Added a gear"));
  MainWindow::getInstance()->pushUndoCommand(com);
  
  return gear;
 
}

MetaGear *SchemaGui::addMetaGear(QString filename, QPointF pos)
{    
  return NULL;
/*  MetaGear *metaGear = _schema->addMetaGear(filename);    

  if (metaGear==NULL)
    return NULL;
  
  //GearGui *gearGui = metaGear->getGearGui();    
  //addItem(gearGui);
  //gearGui->setPos(pos);    
  //gearGui->setZValue(0);
  //gearGui->show();
  update();

  return metaGear;
 */
}


void SchemaGui::renameGear(GearGui *gearGui, QString newName)
{
  if (!gearGui)
    return;

  _schema->renameGear(*gearGui->gear(), newName);
}

MetaGear* SchemaGui::newMetaGear(QPointF pos)
{ 
  /*
  MetaGear *metaGear = _schema->newMetaGear();    
  GearGui *gearGui = metaGear->getGearGui();    

  addItem(gearGui);
  gearGui->setZValue(0);
  gearGui->setPos(pos);
  gearGui->show();
  update();
  
  return metaGear;*/
  return NULL;
}

void SchemaGui::removeGear(GearGui* gearGui)
{  
  Gear* gear = gearGui->gear();

  _schema->removeDeepGear(gear);
  // let the "Schema" signal (gearRemoved) call carRemoved 
  // to do the actual GearGui deletion
  update();
}

void SchemaGui::clear()
{
  QList<QGraphicsItem *> l=items();
  QList<GearGui*> gearGuis;

  //first fill a vector with only gearGuis
  //we have to make it this way because we cannot
  //iterate on QGraphicsItem while removing them at the sametime
  foreach(QGraphicsItem * it,l) { 
    if ( qgraphicsitem_cast<GearGui*>(it))    
      gearGuis<<(GearGui*)(it);
  }
  
  //now remove gearGuis
  foreach(GearGui* ggui,gearGuis)  
    removeGear(ggui);
}

bool SchemaGui::load(QDomElement& parent, Drone::LoadingModeFlags lmf)
{
  clear();
  if(_schema->load(parent, lmf))
  {
    setSchema(_schema);
    return true;
  }
  return false;
}

bool SchemaGui::save(QDomDocument& doc, QDomElement &parent, bool onlySelected)
{
  return _schema->save(doc, parent,onlySelected);
}

void SchemaGui::selectionHasChanged()
{
  QList<QGraphicsItem *> list(selectedItems());
  QGraphicsItem* el;
  GearGui* selectedGear=0;
  qreal zOffset;
  if(_selectionChangeNotificationBypass)
    return;
  _maxZValue++;


  std::cerr<<"Selection has changed. New: "<<list.count()<<std::endl;
  foreach(el,list)
  {
    // create pseudo "layers" with slices of Z values so that comments
    // are always below gears (TODO/UPCOMING) and connections always on top 
    if(qgraphicsitem_cast<ConnectionItem*>(el))
      zOffset=1000000;
    else zOffset=0;
    selectedGear=qgraphicsitem_cast<GearGui*>(el);
    el->setZValue(_maxZValue+zOffset);
  }
  if(selectedGear)
    emit gearSelected(selectedGear);
}

void SchemaGui::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  QList<QGraphicsItem *> list(items(event->scenePos()));
  QList<QGraphicsItem *> selectedList(selectedItems());
  QPointF pos;
  GearGui* gearGui;
  PlugBox* selectedPlugBox;
  QGraphicsItem *el, *el2;

  foreach(el, list)
  {
    if ((gearGui = qgraphicsitem_cast<GearGui*>(el)))
    {
      pos = gearGui->mapFromScene(event->scenePos());
      selectedPlugBox = gearGui->plugHit(pos);
      if (selectedPlugBox)
      {
        _connecting = true;
        _activeConnection = new ConnectionItem();

        _activeConnection->setSourcePlugBox(selectedPlugBox);
        _activeConnection->setConnectionLineEndPoint(event->scenePos());
        addItem(_activeConnection);
        event->accept();
        return;
      }
    }

    //  see : http://harmattan-dev.nokia.com/docs/library/html/qt4/tools-undoframework-diagramscene-cpp.html
    // looks like we're starting an item(s) move, Yay! Let's capture some data to build the undo Command
    if (_moving==No && /*!event->modifiers() &&*/ event->button() == Qt::LeftButton && (el->flags() & QGraphicsItem::ItemIsMovable))
    {
      _moving = Pre;
      //std::cerr<<"About to move"<<std::endl;

    }
  }
  QGraphicsScene::mousePressEvent(event);

}

void SchemaGui::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  QList<QGraphicsItem *> selectedList(selectedItems());
  QGraphicsItem* extra;
  GearGui* gearGui;
  QGraphicsItem *el;
  if (_connecting)
  {
    _activeConnection->setConnectionLineEndPoint(event->scenePos());
  }
  if (_moving == Pre)
  {
    _moving = Yes;
    _movingItems.clear();

    // mimick QGraphicsScene quirky behavior. Starting a move while Ctrl-clicking on
    // an item will select it and move it AFTER we're out of this handler
    if (event->modifiers() & Qt::ControlModifier)
    {
      extra = itemAt(event->scenePos());
      gearGui = qgraphicsitem_cast<GearGui*>(extra);
      if (gearGui && !gearGui->isSelected())
        selectedList << gearGui;
    }
    
    foreach(el, selectedList)
    {
      gearGui = qgraphicsitem_cast<GearGui*>(el);
      if (gearGui)qDebug() << gearGui->gear()->name();
      if (gearGui && (el->flags() & QGraphicsItem::ItemIsMovable))
        _movingItems << gearGui->gear()->getUUID();
    }
    //std::cerr << "mod"<<event->modifiers()<<" Looks like we're moving" << _movingItems.count()<<std::endl;
    _movingStartPos = event->scenePos();

  }

  QGraphicsScene::mouseMoveEvent(event);
}

void SchemaGui::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
  QList<QGraphicsItem *> list(items(event->scenePos()));
  ConnectionItem* ci;
  QGraphicsItem* el;

  foreach(el, list)
  {
    qDebug()<< "found item";

    if ((ci = qgraphicsitem_cast<ConnectionItem*>(el)))
    {
      qDebug()<< "disconnect";
      CommandGeneric* com = new CommandGeneric();

      disconnect(ci->sourcePlugBox(), ci->destPlugBox());
          com->saveSnapshotAfter();
          com->setText(QString("Disconnected gears"));
          MainWindow::getInstance()->pushUndoCommand(com);
      return;
    }
  }

}

void SchemaGui::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  QPointF dist(event->scenePos() - _movingStartPos);
  if (_connecting)
  {
    QList<QGraphicsItem *> list(items(event->scenePos()));
    QPointF pos;
    PlugBox* destPlug;
    QGraphicsItem* el;

    foreach(el, list)
    {
      if (qgraphicsitem_cast<GearGui*>(el))
      {
        pos = (static_cast<GearGui*>(el))->mapFromScene(event->scenePos());
        destPlug = (static_cast<GearGui*>(el))->plugHit(pos);
        if (destPlug)
        {
          _connecting = false;
          CommandGeneric* com = new CommandGeneric();
          
          connect(_activeConnection->sourcePlugBox(), destPlug);
          
          com->saveSnapshotAfter();
          com->setText(QString("Connected gears"));
          MainWindow::getInstance()->pushUndoCommand(com);
          delete _activeConnection;
          return;
        }
      }
    }
    delete _activeConnection;
    _connecting=false;
  }
  else if(_moving==Yes)
  {
    //std::cerr<<"Moved "<<_movingItems.count()<<" items by "<<dist.x()<<","<<dist.y()<<std::endl;
    //new CommandMoveItems(_movingItems,dist);
    emit itemsMoved(_movingItems, dist);

    _moving=No;
  }
  // happens when we didn't move
  else if(_moving==Pre)
  {
    _moving=No;
  }
  QGraphicsScene::mouseReleaseEvent( event);
}

bool SchemaGui::connect(PlugBox *plugA, PlugBox *plugB)
{
  if (!plugA || !plugB)
    return false;

  if (!plugA->canConnectWith(plugB))
    return false;
  
  //in the case of an input that is already
  //connected, we first need to disconnect
  if (plugA->inOut()==IN && plugA->connected())
  {
    disconnectAll(plugA);
  }
  else if (plugB->inOut()==IN && plugB->connected())
  {    
    disconnectAll(plugB);
  }
   
  // tell the schema to make the connection.
  // the graphical connection will be made whn we receive back the signal 
  return _schema->connect(*plugA->plug(), *plugB->plug());
}

GearGui* SchemaGui::getGearGuiByUUID(QString uuid)
{
  Gear* g(_schema->getGearByUUID(uuid));
  GearGui* ggui;
  if(g && (ggui=(GearGui*)g->getGearGui()))
    return ggui;
  else
    return NULL;
}

// retrieves a pair of PlugBox* represented by  a connection
QPair<PlugBox*, PlugBox*> SchemaGui::getPlugBoxesFromConnection(Connection c)
{
  GearGui* g1(getGearGuiByUUID(c.gearA()));
  GearGui* g2(getGearGuiByUUID(c.gearB()));
  QPair<PlugBox*,PlugBox*> nullPair(qMakePair(static_cast<PlugBox*>(NULL), static_cast<PlugBox*>(NULL)));

  if (!g1 || !g2)
    return nullPair;

  PlugBox *pb1(g1->getPlugBox(c.output()));
  PlugBox *pb2(g2->getPlugBox(c.input()));

  if (!pb1 || !pb2)
    return nullPair;

  return qMakePair(pb1, pb2);
}


void SchemaGui::slotConnectionCreated(Schema &schema, Connection c)
{
  QPair<PlugBox*, PlugBox*> conn(getPlugBoxesFromConnection(c));
  
  ConnectionItem* ci(conn.first->connect(conn.second));
  if(ci && _autoSelectNewElements)
    ci->setSelected(true);
  update();

}


void SchemaGui::slotConnectionRemoved(Schema &schema, Connection c)
{
  QPair<PlugBox*, PlugBox*> conn(getPlugBoxesFromConnection(c));
  
  conn.first->disconnect(conn.second);
  update();

}


void SchemaGui::disconnect(PlugBox *plugA, PlugBox *plugB)
{
  if (!plugA || !plugB)
    return;
  
  //disconnect plugbox
  plugA->disconnect(plugB);
  
  //tell the schema to disconnect plugs
  _schema->disconnect(*plugA->plug(), *plugB->plug());  

  //update the canvas
  update();
}

void SchemaGui::disconnectAll(PlugBox *plugBox)
{
  if (!plugBox)
    return;
  
  plugBox->disconnectAll();

  update();
  
  //tell the schema to disconnect all from this plug
  _schema->disconnectAll(*plugBox->plug());
}

QList<GearGui*> SchemaGui::getAllGears()
{
  QList<GearGui*> gg;
  QList<QGraphicsItem*> l=items();
  foreach(QGraphicsItem* it,l)
    if ( qgraphicsitem_cast<GearGui*>(it))    
      gg<<((GearGui*)(it));
  return gg;
}

QList<GearGui*> SchemaGui::getSelectedGears()
{
  QList<GearGui*> gg;
  QList<QGraphicsItem*> l=selectedItems();
  foreach(QGraphicsItem * it,l)
    if ( qgraphicsitem_cast<GearGui*>(it))    
      gg<<((GearGui*)(it));
  return gg;
}

// Warning : order is not preserved ! 
QList<QGraphicsItem*> SchemaGui::getItemsByUUID(QList<QString>& list)
{
  QList<QGraphicsItem*> all(items()),selection;
  GearGui* ggui;
  
  foreach(QGraphicsItem* el,all) 
  {
    // forced to do this since all elements don't have a common interface with name()
    if((ggui=qgraphicsitem_cast<GearGui*>(el))
            && list.contains(ggui->gear()->getUUID()))
      selection<<el;
  }
  return selection;
}

void SchemaGui::moveItemsBy(QList<QGraphicsItem*> list,QPointF delta)
{
  QGraphicsItem* el;
  foreach(el,list)
  {
    // don't move ConnectionItems because they move naturally with the gears
    // they are connected to
    if(!qgraphicsitem_cast<ConnectionItem*>(el))
      el->moveBy(delta.x(),delta.y());
  }
  update();
}

