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
#include "GearGui.h"
#include "ConnectionItem.h"

const int SchemaGui::DEFAULT_CANVAS_SIZE_X = 2048;
const int SchemaGui::DEFAULT_CANVAS_SIZE_Y = 2048;


SchemaGui::SchemaGui(Schema *schema, Engine *engine) :
  QCanvas(DEFAULT_CANVAS_SIZE_X, DEFAULT_CANVAS_SIZE_Y),
  _schema(schema),
  _engine(engine)
{
  
  //todo various background for metagear and main schema
  setBackgroundColor(QColor(107,124,153));

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
  ConnectionItem *connectionItem=NULL;
  PlugBox *sourcePlugBox;
  PlugBox *destPlugBox;
  Gear *gearA;
  Gear *gearB;

  for (std::list<Schema::Connection*>::iterator it = connections.begin(); it != connections.end(); ++it)
  {
    connectionItem = new ConnectionItem(this);

    gearA = _schema->getGearByName((*it)->gearA());
    sourcePlugBox = gearA->getGearGui()->getOutputPlugBox((*it)->output());

    gearB = _schema->getGearByName((*it)->gearB());
    destPlugBox = gearB->getGearGui()->getInputPlugBox((*it)->input());

    connectionItem->createConnectionLineOnly(sourcePlugBox, destPlugBox);

    connectionItem->show();

    delete (*it);//free

  }

  update();
}

SchemaGui::~SchemaGui()
{
}

void SchemaGui::addGear(std::string type, int x, int y)
{    
  addGear(type, _schema->getNewGearName(type), x, y);
}

void SchemaGui::addGear(std::string type, std::string name, int x, int y)
{            
  Gear *gear = _schema->addGear(_engine, type, name);    

  if (!gear)
    return;
    
  GearGui *gearGui = gear->getGearGui();    

  gearGui->setCanvas(this);    
  gearGui->move(x,y);    
  gearGui->setZ(0);
  gearGui->show();
  update();
}

void SchemaGui::addMetaGear(std::string type, int x, int y)
{    
  addMetaGear(type, _schema->getNewGearName(type), x, y);
}

void SchemaGui::addMetaGear(std::string type, std::string name, int x, int y)
{            
  Gear *gear = _schema->addMetaGear(_engine, type, name);    
  GearGui *gearGui = gear->getGearGui();    

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

  _engine->scheduleGearDeletion(gear);

  update();
}

void SchemaGui::clearSchema()
{

  QCanvasItemList l=allItems();

  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
    (*it)->hide();


  //_schema->clear();
  update();

}

void SchemaGui::loadSchema(std::string filename)
{
}

void SchemaGui::saveSchema(std::string filename)
{
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

void SchemaGui::moveGearBy(GearGui *gearGui, int x, int y)
{
  gearGui->moveBy(x, y); 
  update();    
}
