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

#include <QGraphicsScene.h>
#include <QFileInfo>
#include <QMenu>
#include "Schema.h"
#include "ISchemaEventListener.h"

class GearGui;
class PlugBox;
class Engine;
class ConnectionItem;
class QDomDocument;
class GearListMenu;
class MetaGearListMenu;
class SchemaEditor;

class SchemaGui : public QGraphicsScene, public ISchemaEventListener
{
  Q_OBJECT
  
public:
  enum MovingState {No,Pre,Yes};
  SchemaGui(Schema *schema, Engine *engine);
  ~SchemaGui();

  //Gear* addGear(std::string type, std::string name, int x, int y);
  Gear* addGear(std::string type, QPointF pos);
  MetaGear* addMetaGear(std::string filename, QPointF pos);  
  MetaGear* newMetaGear(QPointF pos);  
  void renameGear(GearGui *gearGui, std::string newName);
  void removeGear(GearGui* gearGui);
    
  
  void onGearAdded(Schema *schema, Gear *gear);
  void onGearRemoved(Schema *schema, Gear *gear);

  void setSchemaEditor(SchemaEditor* se);
  SchemaEditor* getSchemaEditor() const;
  bool connect(PlugBox *plugA, PlugBox *plugB);
  void disconnect(PlugBox *plugA, PlugBox *plugB);
  void disconnectAll(PlugBox *plugBox);

  void clear();
  bool load(QDomElement& parent);
  bool save(QDomDocument& doc, QDomElement &parent, bool onlySelected=false);

  std::vector<GearGui*> getAllGears();
  std::vector<GearGui*> getSelectedGears();

  Schema * getSchema(){return _schema;}
  void rebuildSchema();

  void setSelectionChangeBypass(bool v){_selectionChangeBypass=v;}
  QList<QGraphicsItem*> getItemsByName(QList<QString>& list);
  
  void moveItemsBy(QList<QGraphicsItem*> list,QPointF delta);
  
signals:
  void gearSelected(GearGui *gearGui);  
  void itemsMoved(QList<QString> &itemNames, QPointF dist);

public slots:
  void selectionHasChanged();

protected:

  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
  void mousePressEvent(QGraphicsSceneMouseEvent * event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

  void drawBackground ( QPainter * painter, const QRectF & rect );
  
  enum ePlugContextItemId
  {
    EXPOSE, UNEXPOSE
  };

  //popupmenus  
  SchemaEditor* _schemaEditor;
  
private:
  static const int DEFAULT_CANVAS_SIZE_X;
  static const int DEFAULT_CANVAS_SIZE_Y;
  
  void setSchema(Schema *schema);

  ConnectionItem* _activeConnection;
  bool _connecting;

  
  Schema *_schema;
  Engine *_engine;
  qreal _maxZValue;

  // when selecting all items, used to bypass all single selectionChange signals but one 
  bool _selectionChangeBypass;

  // all this stuff is to manually save item move info for the undo stack
  MovingState _moving;
  QList<QString> _movingItems;
  QPointF _movingStartPos;
};

#endif
