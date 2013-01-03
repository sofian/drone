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

#include <qrect.h>
#include <q3popupmenu.h>
#include <qfileinfo.h>
//Added by qt3to4:
#include <QWheelEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QKeyEvent>
#include <string>
#include <vector>
#include <QGraphicsView>
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

  void zoom(float factor);

  void addGear(std::string name, int posX, int posY);
  void addMetaGear(std::string filename, int posX, int posY);
  void addNewMetaGear(int posX, int posY);
  void removeGear(GearGui *gear);

public slots:
  void slotMenuGearSelected(QString name);
  void slotMenuMetaGearSelected(QFileInfo* metaGearFileInfo);
  
  //common slots  
  void zoomIn();
  void zoomOut();

  void slotGearProperties();
  void slotGearDelete();
  void slotGearSelectAll();
  void slotGearCopy();
  void slotGearPaste();
  void slotNewMetaGear();

  //plug editing slots
  void slotPlugExpose();
  void slotPlugUnexpose();
  
  //metagear editing slots
  void slotSaveMetaGear();

signals:
		void gearSelected(GearGui *gearGui);

protected:

  void keyPressEvent ( QKeyEvent * e );
  void keyReleaseEvent ( QKeyEvent * e );

  void contentsMousePressEvent(QMouseEvent *mouseEvent);
  void contentsMouseMoveEvent(QMouseEvent *mouseEvent);       
  void contentsMouseReleaseEvent(QMouseEvent *mouseEvent);
  void contentsWheelEvent(QWheelEvent *wheelEvent);    
  void contentsMouseDoubleClickEvent(QMouseEvent *mouseEvent);
  void contextMenuEvent(QContextMenuEvent *contextMenuEvent);
  void dropEvent(QDropEvent* event);
  void dragEnterEvent(QDragEnterEvent* event);
  void dragMoveEvent ( QDragMoveEvent * event );
  
  void drawBackground ( QPainter * painter, const QRectF & rect );

  void setupMatrix();
  
  void deleteSelectedGears();
  void unselectAllGears();
  void selectAllGears();
  void selectOneGear(GearGui* gear);
  void selectGearsInRectangle(QRect rect);
  QRectF getBoundingBoxOfAllSelectedGears();
  // of all selected gears, returns the one that is at the top left of the bounding rect of all gears
  Gear* getTopLeftSelectedGear();
  void moveSelectedGearsBy(int x, int y);
  void toggleGearSelection(GearGui* gear);

private:
  
  void associateControlPanelWithMetaGear(MetaGear *metaGear);

  static const std::string NAME;
  static const double ZOOM_FACTOR;
  
  enum ePlugContextItemId
  {
    EXPOSE, UNEXPOSE
  };

  Engine *_engine;
  SchemaGui *_schemaGui;
  GearGui *_movingGear;
  QPointF _movingGearStartPos;
  QPointF _selectBoxStartPos;
  double _scale;

  //popupmenus  
  Q3PopupMenu *_contextMenu; 
  GearListMenu *_gearListMenu; 
  MetaGearListMenu *_metaGearListMenu; 
  
  // position of right click in Canvas coordinates
  QPoint _contextMenuPos;  
  Q3PopupMenu *_gearContextMenu;
  Q3PopupMenu *_metaGearContextMenu;
  Q3PopupMenu *_plugContextMenu;
  GearGui *_contextGear;//when the context menu of a gear is pop, this is the gear that make the menu pop
  PlugBox *_contextPlug; // when the context menu of a plug is pop, this is the plug that make the menu pop

  PanelScrollView *_panelScrollView;


  QGraphicsRectItem *_selectBox;
};

#endif
