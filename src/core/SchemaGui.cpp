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

const int SchemaGui::DEFAULT_CANVAS_SIZE_X = 2048;
const int SchemaGui::DEFAULT_CANVAS_SIZE_Y = 2048;


SchemaGui::SchemaGui(Schema *schema, Engine *engine) :
  QCanvas(DEFAULT_CANVAS_SIZE_X, DEFAULT_CANVAS_SIZE_Y),
  _engine(engine)
{
  
  //todo various background for metagear and main schema
  setBackgroundColor(QColor(107,124,153));
  
  setSchema(schema);

}

SchemaGui::~SchemaGui()
{
}

void SchemaGui::setSchema(Schema *schema)
{
  clear();

  _schema = schema;

  //add gearguis
  std::list<Gear*> gears = _schema->getGears();

  GearGui *gearGui=NULL;
  for (std::list<Gear*>::iterator it=gears.begin();it!=gears.end();++it)
  {
		gearGui=(*it)->getGearGui();
		gearGui->setCanvas(this);
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

void SchemaGui::addGear(std::string type, int x, int y)
{            
  Gear *gear = _schema->addGear(type);    

  if (!gear)
    return;
    
  GearGui *gearGui = gear->getGearGui();    

  gearGui->setCanvas(this);    
  gearGui->move(x,y);    
  gearGui->setZ(0);
  gearGui->show();
  update();
}

void SchemaGui::addMetaGear(std::string name, int x, int y)
{    
  MetaGear *metaGear = _schema->addMetaGear(name);    
  GearGui *gearGui = metaGear->getGearGui();    

  gearGui->setCanvas(this);    
  gearGui->move(x,y);    
  gearGui->setZ(0);
  gearGui->show();
  update();
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
  QCanvasItemList l=allItems();
  std::vector<GearGui*> gearGuis;

  //first fill a vector with only gearGuis
  //we have to make it this way because we cannot
  //iterate on QCanvasItemList while removing them at the sametime
  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
    if ( (*it)->rtti() == GearGui::CANVAS_RTTI_GEAR)    
      gearGuis.push_back((GearGui*)(*it));
      
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

bool SchemaGui::save(QDomDocument& doc, QDomElement &parent)
{
  return _schema->save(doc, parent);
}

GearGui* SchemaGui::testForGearCollision(const QPoint &p)
{     
  QCanvasItemList l=collisions(p);

  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
  {
    if ( (*it)->rtti() == GearGui::CANVAS_RTTI_GEAR)
    {
      return(GearGui*)(*it);
    }
  }
  return NULL;
}

ConnectionItem* SchemaGui::testForConnectionCollision(const QPoint &p)
{        
  QCanvasItemList l=collisions(p);

  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
  {
    if ( (*it)->rtti() == ConnectionItem::CANVAS_RTTI_CONNECTION)
      return(ConnectionItem*) (*it);
  }
  return NULL;
}

void SchemaGui::unHilightAllConnections()
{        
  QCanvasItemList l=allItems();
  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
  {
    if ( (*it)->rtti() == ConnectionItem::CANVAS_RTTI_CONNECTION)
    {
      ((ConnectionItem*)(*it))->hiLight(false);
    }
  }    
}

void SchemaGui::unHilightAllPlugBoxes()
{        
  QCanvasItemList l=allItems();
  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
  {
    if ( (*it)->rtti() == GearGui::CANVAS_RTTI_GEAR)
    {
      ((GearGui*)(*it))->unHilightAllPlugBoxes();
    }
  }    
}


void SchemaGui::moveGearBy(GearGui *gearGui, int x, int y)
{
  gearGui->moveBy(x, y); 
  update();    
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
  
  //update the canvas
  update();
  
  //tell the schema to disconnect plugs
  _schema->disconnect(plugA->plug(), plugB->plug());  
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

