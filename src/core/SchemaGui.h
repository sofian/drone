/* SchemaGui.h
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

#ifndef SCHEMAEGUI_INCLUDED
#define SCHEMAEGUI_INCLUDED

#include <q3canvas.h>

#include "Schema.h"

class GearGui;
class PlugBox;
class Engine;
class ConnectionItem;
class QDomDocument;
             
class SchemaGui : public Q3Canvas
{
public:
  SchemaGui(Schema *schema, Engine *engine);
  ~SchemaGui();

  //Gear* addGear(std::string type, std::string name, int x, int y);
  Gear* addGear(std::string type, int x, int y);
  MetaGear* addMetaGear(std::string filename, int x, int y);  
  MetaGear* newMetaGear(int x, int y);  
  void renameGear(GearGui *gearGui, std::string newName);
  void removeGear(GearGui* gearGui);
    
  bool connect(PlugBox *plugA, PlugBox *plugB);
  void disconnect(PlugBox *plugA, PlugBox *plugB);
  void disconnectAll(PlugBox *plugBox);

  void clear();
  bool load(QDomElement& parent);
  bool save(QDomDocument& doc, QDomElement &parent, bool onlySelected=false);

  void moveGearBy(GearGui *gearItem, int x, int y);
  GearGui* testForGearCollision(const QPoint &p);
  ConnectionItem* testForConnectionCollision(const QPoint &p);
  void unHilightAllConnections();
  void unHilightAllPlugBoxes();

  std::vector<GearGui*> getAllGears();
  std::vector<GearGui*> getSelectedGears();

  Schema * getSchema(){return _schema;}
  void rebuildSchema();
  
private:
  static const int DEFAULT_CANVAS_SIZE_X;
  static const int DEFAULT_CANVAS_SIZE_Y;
  
  void setSchema(Schema *schema);

  Schema *_schema;
  Engine *_engine;
};

#endif
