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

#include <qpopupmenu.h>
#include <string>
#include <vector>

#include "SchemaGui.h"

class GearGui;
class ConnectionItem;
class Engine;
class Schema;
class GearListMenu;

class SchemaEditor : public QCanvasView
{
  Q_OBJECT

public:

  enum eStatus
  {
    IDLE, MOVING_GEAR, CONNECTING
  };

  SchemaEditor(QWidget *parent, SchemaGui *schemaGui, Engine * engine);
  ~SchemaEditor();
  
  void zoomIn();
  void zoomOut();
  void zoom(float factor);

public slots:
  void slotMenuGearSelected(QString name);
  void slotGearProperties();
  void slotGearDelete();

  //metagear editing slots
  void slotNewSchema();
  void slotLoadSchema();
  void slotSaveSchema();
  void slotAddMetaGear();

protected:

  void keyPressEvent ( QKeyEvent * e );
  void keyReleaseEvent ( QKeyEvent * e );

  void contentsMousePressEvent(QMouseEvent *mouseEvent);
  void contentsMouseMoveEvent(QMouseEvent *mouseEvent);       
  void contentsMouseReleaseEvent(QMouseEvent *mouseEvent);
  void contentsWheelEvent(QWheelEvent *wheelEvent);    
  void contentsMouseDoubleClickEvent(QMouseEvent *mouseEvent);
  void contextMenuEvent(QContextMenuEvent *contextMenuEvent);

private:
  static const std::string NAME;
  static const double ZOOM_FACTOR;

  Engine *_engine;
  SchemaGui *_schemaGui;
  eStatus _state;
  GearGui *_movingGear;
  QPoint _movingGearStartPos;
  double _zoom;

  ConnectionItem* _activeConnection;

  //popupmenus  
  QPopupMenu *_contextMenu; 
  GearListMenu *_gearListMenu; 
  
  // position of right click in Canvas coordinates
  QPoint _contextMenuPos;  
  QPopupMenu *_gearContextMenu;
  GearGui *_contextGear;//when the context menu of a gear is pop, this is the gear that make the menu pop
};

#endif
