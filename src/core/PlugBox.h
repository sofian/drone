/* PlugBox.h
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

#ifndef PLUGBOX_INCLUDED
#define PLUGBOX_INCLUDED

#include <qpainter.h>
#include <string>
#include <vector>
class Engine;
class AbstractPlug;
class GearGui;
class ConnectionItem;


class PlugBox
{
public:
  enum eStatus
  {
    NORMAL, HILIGHTED
  };

  static const int PLUGBOX_SIZE;

  PlugBox(AbstractPlug *plug, GearGui *gearItem, Engine *engine);
  virtual ~PlugBox();

  QColor color();
  void draw(int x, int y, int gearSizeX, QPainter &painter);
  bool hitted(int x, int y);
  void hilight(bool hiLight);

  bool connect(PlugBox *plugBox, ConnectionItem *connectionItem);
  void assignConnectionOnly(PlugBox *plugBox, ConnectionItem *connectionItem);
  bool canConnectWith(PlugBox *plugBox);    
  void disconnect(ConnectionItem *connectionItem, bool deleteConnectionItem=true);
  void disconnectAll();

  AbstractPlug *plug(){return _plug;};

  std::string test(){return "test";};


  int connectionHandleX();
  int connectionHandleY();

  int x(){return _x;};
  int y(){return _y;};


private:    

  void doHilight();
  void doUnlight();

  AbstractPlug *_plug;   
  GearGui *_gearGui;
  Engine *_engine;
  int _x, _y;    
  eStatus _status;
  int _hilightScaling;

  std::vector<ConnectionItem*> _connectionItems;

  static const int CONNECTION_HANDLE_OFFSETX;
  static const int CONNECTION_HANDLE_OFFSETY;
  static const int PLUG_NAME_NB_CHARS;
  static const int MAX_HILIGHTSCALING;

  //fonts
  static const QFont SHORTNAME_FONT;
  static const QFont SHORTNAME_FONT_BOLD;

};

#endif
