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

#include <qcanvas.h>

#include "Schema.h"

class GearGui;
class Engine;
class ConnectionItem;
             
class SchemaGui : public QCanvas
{
public:
  SchemaGui(Schema *schema, Engine *engine);
  ~SchemaGui();

  void addGear(std::string type, std::string name, int x, int y);
  void addGear(std::string type, int x, int y);
  void addMetaGear(std::string type, std::string name, int x, int y);
  void addMetaGear(std::string type, int x, int y);
  void removeGear(GearGui* gearGui);
  
  void clearSchema();
  void loadSchema(std::string filename);
  void saveSchema(std::string filename);

  void moveGearBy(GearGui *gearItem, int x, int y);
  GearGui* testForGearCollision(const QPoint &p);
  ConnectionItem* testForConnectionCollision(const QPoint &p);
  void unHilightAllConnections();

private:
  static const int DEFAULT_CANVAS_SIZE_X;
  static const int DEFAULT_CANVAS_SIZE_Y;
  
  Schema *_schema;
  Engine *_engine;
};

#endif
