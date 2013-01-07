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
#include "MetaGear.h"
#include "GearGui.h"
#include "PlugBox.h"
#include "ConnectionItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QVarLengthArray>
#include <q3filedialog>
#include <qtextstream>
const int SchemaGui::DEFAULT_CANVAS_SIZE_X = 2048;
const int SchemaGui::DEFAULT_CANVAS_SIZE_Y = 2048;


SchemaGui::SchemaGui(Schema *schema, Engine *engine) :
  QGraphicsScene(0,0,DEFAULT_CANVAS_SIZE_X, DEFAULT_CANVAS_SIZE_Y),
  _activeConnection(NULL),
  _connecting(false),
  _engine(engine)
{
  
  //todo various background for metagear and main schema
  //setBackgroundColor(QColor(107,124,153));
  QRadialGradient gradient(0, 0, 10);
  gradient.setSpread(QGradient::RepeatSpread);
  //setBackgroundBrush(gradient);

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
  rebuildSchema();
}

void SchemaGui::rebuildSchema()
{
  //add gearguis
  std::list<Gear*> gears = _schema->getGears();

  GearGui *gearGui=NULL;
  for (std::list<Gear*>::iterator it=gears.begin();it!=gears.end();++it)
  {
		gearGui=(*it)->getGearGui();
		addItem(gearGui);
    gearGui->show();
  }
	
  //add connectionItems
  std::list<Schema::Connection*> connections;
  _schema->getAllConnections(connections);
  
  PlugBox *sourcePlugBox;
  PlugBox *destPlugBox;
  Gear *gearA;
  Gear *gearB;

  for (std::list<Schema::Connection*>::iterator it = connections.begin(); it != connections.end(); ++it)
  {   
    gearA = _schema->getGearByName((*it)->gearA());
    gearB = _schema->getGearByName((*it)->gearB());

    if (gearA && gearB)
		{
			sourcePlugBox = gearA->getGearGui()->getOutputPlugBox((*it)->output());
			destPlugBox = gearB->getGearGui()->getInputPlugBox((*it)->input());
			sourcePlugBox->connect(destPlugBox);
		}
		
    delete (*it);//free temporary Connection
  }

  update();

}



  void SchemaGui::drawBackground ( QPainter * painter, const QRectF & rect )
  {
    painter->fillRect(rect,QColor(50,50,50));
    QVarLengthArray<QLineF, 36> lines;
        for (int i = 0; i <= 20; i ++) {
            lines.append(QLineF(i*100+0.5,0,i*100+0.5,10000));
            lines.append(QLineF(0,i*100+0.5,10000,i*100+0.5));
        }
    painter->setPen(QPen(QColor(70,70,70),0.5));
    painter->drawLines(lines.data(), lines.size());
  }

  
Gear* SchemaGui::addGear(std::string type, QPointF pos)
{            
  Gear *gear = _schema->addGear(type);    

  if (!gear)
    return NULL;
    
  GearGui *gearGui = gear->getGearGui();    

  addItem(gearGui);    
  gearGui->setPos(pos);    
  gearGui->setZValue(0);
  gearGui->show();
  update();

  return gear;
}

MetaGear *SchemaGui::addMetaGear(std::string filename, QPointF pos)
{    
  MetaGear *metaGear = _schema->addMetaGear(filename);    

  if (metaGear==NULL)
    return NULL;
  
  GearGui *gearGui = metaGear->getGearGui();    
  addItem(gearGui);
  gearGui->setPos(pos);    
  gearGui->setZValue(0);
  gearGui->show();
  update();

  return metaGear;
}


void SchemaGui::renameGear(GearGui *gearGui, std::string newName)
{
  if (!gearGui)
    return;

  _schema->renameGear(gearGui->gear(), newName);
}

MetaGear* SchemaGui::newMetaGear(QPointF pos)
{    
  MetaGear *metaGear = _schema->newMetaGear();    
  GearGui *gearGui = metaGear->getGearGui();    

  addItem(gearGui);
  gearGui->setZValue(0);
  gearGui->setPos(pos);
  gearGui->show();
  update();

  return metaGear;
}

void SchemaGui::removeGear(GearGui* gearGui)
{  
  Gear* gear = gearGui->gear();

  delete gearGui;

  _schema->removeDeepGear(gear);

  update();
}

void SchemaGui::clear()
{
  QList<QGraphicsItem *> l=items();
  std::vector<GearGui*> gearGuis;

  //first fill a vector with only gearGuis
  //we have to make it this way because we cannot
  //iterate on QGraphicsItem while removing them at the sametime
  foreach(QGraphicsItem * it,l) { 
    if ( qgraphicsitem_cast<GearGui*>(it))    
      gearGuis.push_back((GearGui*)(it));
  }
  
  //now remove gearGuis
  for (std::vector<GearGui*>::iterator it=gearGuis.begin();it!=gearGuis.end();++it)  
    removeGear((*it));
}

bool SchemaGui::load(QDomElement& parent)
{
  clear();
  if(_schema->load(parent))
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

void SchemaGui::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  QList<QGraphicsItem *> list(items(event->scenePos()));
  QPointF pos;
  PlugBox* selectedPlugBox;
  QGraphicsItem* el;
  foreach(el,list)
  {
    if(qgraphicsitem_cast<GearGui*>(el))
    {
      pos = (static_cast<GearGui*>(el))->mapFromScene(event->scenePos());
      selectedPlugBox = (static_cast<GearGui*>(el))->plugHit(pos);
      if (selectedPlugBox)
      {
        _connecting = true;
        _activeConnection = new ConnectionItem();

        _activeConnection->setSourcePlugBox(selectedPlugBox);
        addItem(_activeConnection);
        event->accept();
        return;
      }
    }
  }
  QGraphicsScene::mousePressEvent(event);
}


void SchemaGui::mouseMoveEvent( QGraphicsSceneMouseEvent * event)
{
  
  if(_connecting)
  {
    _activeConnection->setConnectionLineEndPoint(event->scenePos());

//    gearGui = testForGearCollision(p);   
//
//    if (gearGui!=NULL)
//    {
//      if ( ((selectedPlugBox = gearGui->plugHit(p)) != 0))
//      {      
//        if (_activeConnection->sourcePlugBox()->canConnectWith(selectedPlugBox))
//            gearGui->performPlugHighligthing(selectedPlugBox);
//        else
//            gearGui->unHilightAllPlugBoxes();
//      }
//      else
//        gearGui->unHilightAllPlugBoxes();
//    }

  }
  QGraphicsScene::mouseMoveEvent( event);
}

void SchemaGui::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
        std::cerr<<"mousedbl"<<std::endl;

    QList<QGraphicsItem *> list(items(event->scenePos()));
    ConnectionItem* ci;
    QGraphicsItem* el;
    foreach(el, list)
    {
            std::cerr<<"found item"<<std::endl;

      if (ci=qgraphicsitem_cast<ConnectionItem*>(el))
      {
              std::cerr<<"disconnect"<<std::endl;

        disconnect(ci->sourcePlugBox(), ci->destPlugBox()); 
        return;
      }
    }

}

void SchemaGui::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
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
          connect(_activeConnection->sourcePlugBox(), destPlug);
          delete _activeConnection;
          return;
        }
      }
    }
    delete _activeConnection;
    _connecting=false;
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
   
  //connect plugBox
  plugA->connect(plugB);

  //update the canvas
  update();

  //tell the schema to make the connection
  return _schema->connect(plugA->plug(), plugB->plug());
}

void SchemaGui::disconnect(PlugBox *plugA, PlugBox *plugB)
{
  if (!plugA || !plugB)
    return;
  
  //disconnect plugbox
  plugA->disconnect(plugB);
  
  //tell the schema to disconnect plugs
  _schema->disconnect(plugA->plug(), plugB->plug());  

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
  _schema->disconnectAll(plugBox->plug());
}

std::vector<GearGui*> SchemaGui::getAllGears()
{
  std::vector<GearGui*> vec;
  QList<QGraphicsItem*> l=items();
  foreach(QGraphicsItem* it,l)
    if ( qgraphicsitem_cast<GearGui*>(it))    
      vec.push_back((GearGui*)(it));
  return vec;
}

std::vector<GearGui*> SchemaGui::getSelectedGears()
{
  std::vector<GearGui*> vec;
  QList<QGraphicsItem*> l=selectedItems();
  foreach(QGraphicsItem * it,l)
    if ( qgraphicsitem_cast<GearGui*>(it))    
      vec.push_back((GearGui*)(it));
  return vec;
}
