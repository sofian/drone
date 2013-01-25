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

#include <QGraphicsScene>
#include <QFileInfo>
#include <QMenu>
#include "Schema.h"

class GearGui;
class PlugBox;
class Engine;
class ConnectionItem;
class QDomDocument;
class GearListMenu;
class MetaGearListMenu;
class SchemaEditor;

class SchemaGui : public QGraphicsScene
{
  Q_OBJECT
  
public:
  enum MovingState {No,Pre,Yes};
  SchemaGui(Schema *schema, Engine *engine);
  ~SchemaGui();

  //Gear* addGear(QString type, QString name, int x, int y);
  Gear* addGear(QString type, QPointF pos);
  MetaGear* addMetaGear(QString filename, QPointF pos);  
  MetaGear* newMetaGear(QPointF pos);  
  void renameGear(GearGui *gearGui, QString newName);
  void removeGear(GearGui* gearGui);
    
  void setSchemaEditor(SchemaEditor* se);
  SchemaEditor* getSchemaEditor() const;
  bool connect(PlugBox *plugA, PlugBox *plugB);
  void disconnect(PlugBox *plugA, PlugBox *plugB);
  void disconnectAll(PlugBox *plugBox);

  void clear();
  bool load(QDomElement& parent, Drone::LoadingModeFlags lmf);
  bool save(QDomDocument& doc, QDomElement &parent, bool onlySelected=false);

  QList<GearGui*> getAllGears();
  QList<GearGui*> getSelectedGears();
  GearGui* getGearGuiByUUID(QString uuid);
  QPair<PlugBox*, PlugBox*> getPlugBoxesFromConnection(Connection c);

  void setSelectionChangeNotificationBypass(bool v){_selectionChangeNotificationBypass=v;}
  void setAutoSelectNewElements(bool p){_autoSelectNewElements=p;}

  QPointF getPasteOffset(){return _pasteOffset+=QPointF(100,100);}
  void resetPasteOffset(){_pasteOffset=QPointF(0,0);}
  

  Schema * getSchema(){return _schema;}
  void rebuildSchema();

  QList<QGraphicsItem*> getItemsByUUID(QList<QString>& list);
  
  void moveItemsBy(QList<QGraphicsItem*> list,QPointF delta);
  
signals:
  void gearSelected(GearGui *gearGui);  
  void itemsMoved(QList<QString> &itemNames, QPointF dist);

public slots:
  void selectionHasChanged();
  void onGearAdded(Schema &schema,Gear &gear);
  void slotGearRemoved(Schema &schema,Gear &gear);
  void onConnectionCreated(Schema &schema, Connection conn);
  void onConnectionRemoved(Schema &schema, Connection conn);

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

  QPointF _pasteOffset;
  Schema *_schema;
  Engine *_engine;
  qreal _maxZValue;

  // when selecting all items, used to bypass all single selectionChange signals but one 
  bool _selectionChangeNotificationBypass;

  // all this stuff is to manually save item move info for the undo stack
  MovingState _moving;
  QList<QString> _movingItems;
  QPointF _movingStartPos;
  bool _autoSelectNewElements;
};

#endif
