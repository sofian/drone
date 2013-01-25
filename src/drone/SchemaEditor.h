/* SchemaEditor.h
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


/*

The schema editor inherits QCanvasView. It handles mouse/keyboards events
and dispatches them once they are interpreted to the schemaGui so it can
react properly

*/

#ifndef SCHEMAEDITOR_INCLUDED
#define SCHEMAEDITOR_INCLUDED

#include <QRect>
#include <QMenu>
#include <QFileInfo>
//Added by qt3to4:
#include <QWheelEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QKeyEvent>

#include <QGraphicsView>

#include <string>
#include <vector>
#include "SchemaGui.h"

class GearGui;
class ConnectionItem;
class Engine;
class Schema;
class GearListMenu;
class MetaGearListMenu;
class PanelScrollView;

/**
 * SchemaEditor maintain synchronization between Schema and SchemaGui
 */ 
class SchemaEditor : public QGraphicsView
{
  Q_OBJECT

public:

  SchemaEditor(QWidget *parent, SchemaGui *schemaGui, Engine * engine, PanelScrollView *panelScrollView);
  ~SchemaEditor();
  void buildContextMenus();

  void zoom(float factor);

  void addGear(QString name, QPoint pos);
  void addMetaGear(QString filename, QPoint pos);
  void addNewMetaGear(QPoint pos);

  void setContextGear(GearGui* gui){_contextGear = gui;}
  void setContextPlug(PlugBox* pb){_contextPlug = pb;}
  QMenu* getGearContextMenu() const {return _gearContextMenu;}
  QMenu* getMetaGearContextMenu() const {return _metaGearContextMenu;}
  QMenu* getPlugContextMenu() const {return _plugContextMenu;}
  SchemaGui* getSchemaGui() const {return _schemaGui;}
public slots:

  //common slots  
  void onZoomIn();
  void onZoomOut();

  void onMenuGearSelected(QString name);
  void onMenuMetaGearSelected(QFileInfo* metaGearFileInfo);
  void onGearProperties();
  void onDeleteSelected();
  void onSelectAll();
  void onGearCopy();
  void onGearPaste();
  void onNewMetaGear();

  //plug editing slots
  void onPlugToggleExpose();
  
  //metagear editing slots
  void onSaveMetaGear();

  

protected:

  void keyPressEvent(QKeyEvent *keyEvent);
  void contextMenuEvent(QContextMenuEvent *contextMenuEvent);
  void dragEnterEvent(QDragEnterEvent* event);
  void dragMoveEvent(QDragMoveEvent * event);
  void dropEvent(QDropEvent* event);

  void setupMatrix();
  void deleteSelected();
  
  void selectAll();


  QMenu *_contextMenu; 
  GearListMenu *_gearListMenu; 
  MetaGearListMenu *_metaGearListMenu; 
 
  
  // position of right click in Canvas coordinates
  QPoint _contextMenuPos;  
  QMenu *_gearContextMenu;
  QMenu *_metaGearContextMenu;
  QMenu *_plugContextMenu;
  QAction *_exposePlugAction;
  GearGui *_contextGear;//when the context menu of a gear is pop, this is the gear that make the menu pop
  PlugBox *_contextPlug; // when the context menu of a plug is pop, this is the plug that make the menu pop

  
  
  
private:
  
  void associateControlPanelWithMetaGear(MetaGear *metaGear);

  static const QString NAME;
  static const double ZOOM_FACTOR;
  

  Engine *_engine;
  SchemaGui *_schemaGui;
  double _scale;
  qreal _maxZValue;
  
  // when true, ignore selectionChange signals
  bool _selectionChangeNotificationBypass;

  PanelScrollView *_panelScrollView;
};

#endif
