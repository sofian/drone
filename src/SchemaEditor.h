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

#ifndef SCHEMAEDITOR_INCLUDED
#define SCHEMAEDITOR_INCLUDED

#include <qcanvas.h>
#include <qpopupmenu.h>
#include <string>
#include <vector>

class GearGui;
class ConnectionItem;
class Engine;
class Schema;
class SchemaEditor : public QCanvasView
{
  Q_OBJECT

  public:


  enum eStatus
  {
    IDLE, MOVING_GEAR, CONNECTING
  };

  SchemaEditor(QCanvas *canvas, QWidget *parent, Engine * engine, Schema * schema);
  ~SchemaEditor();


  void addGear(std::string type, std::string name, int x, int y);
  void addGear(std::string type, int x, int y);
  void removeGear(GearGui* gearGui);
  void setSchema(Schema *schema);
  void zoomIn();
  void zoomOut();
  void zoom(float factor);
  void clearSchema();
  void loadSchema(std::string filename);
  void saveSchema(std::string filename);

public slots:
  void slotMenuItemSelected(int id);
  void slotGearProperties();
  void slotGearDelete();

protected:

  void keyPressEvent ( QKeyEvent * e );
  void keyReleaseEvent ( QKeyEvent * e );

  void contentsMousePressEvent(QMouseEvent *mouseEvent);
  void contentsMouseMoveEvent(QMouseEvent *mouseEvent);       
  void contentsMouseReleaseEvent(QMouseEvent *mouseEvent);
  void contentsWheelEvent(QWheelEvent *wheelEvent);    
  void contentsMouseDoubleClickEvent(QMouseEvent *mouseEvent);
  void contextMenuEvent(QContextMenuEvent *contextMenuEvent);
  void moveGearBy(GearGui *gearItem, int x, int y);
  GearGui* testForGearCollision(const QPoint &p);
  ConnectionItem* testForConnectionCollision(const QPoint &p);
  void unHilightAllConnections();

  QPopupMenu* createGearsMenu();

private:
  static const std::string NAME;
  static const double ZOOM_FACTOR;

  Engine *_engine;
  Schema *_schema;
  eStatus _state;
  GearGui *_movingGear;
  QPoint _movingGearStartPos;
  double _zoom;

  ConnectionItem* _activeConnection;

  //popupmenus
  std::vector<std::string> _allGearsName;
  QPopupMenu *_allGearsMenu; 
  // position of right click in Canvas coordinates
  QPoint _allGearsMenuPos;
  QPopupMenu *_gearMenu;
  GearGui *_contextGear;//when the context menu of a gear is pop, this is the gear that make the menu pop
};

#endif
