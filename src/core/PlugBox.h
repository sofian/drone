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
#include <QGraphicsEllipseItem>
#include "AbstractPlug.h"

class GearGui;
class ConnectionItem;

class PlugBox
{
public:
  enum eStatus
  {
    NORMAL, HILIGHTED
  };

  static const int PLUGBOX_RADIUS;

  PlugBox(AbstractPlug *plug, GearGui *gearItem);
  virtual ~PlugBox();

  QColor color();
  void draw(QPainter *painter);
  void drawSelected(QPainter *painter);
  void setPos(QPointF pos){_pos=pos;}
  bool hit(QPointF pos);
  void hilight(bool hiLight);
  static qreal plugNameWidth(std::string name);

  bool connect(PlugBox *plugBox);  
  bool canConnectWith(PlugBox *plugBox);    
  void disconnect(PlugBox *plugBox);
  void disconnectAll();

  AbstractPlug *plug(){return _plug;};


  QPointF connectionHandlePos();
 
  QPointF  pos(){return _pos;};
  QPointF  scenePos();

  //stubs
  eInOut inOut(){return _plug->inOut();}
  bool connected(){return _plug->connected();}
    
private:    

  void doHilight();
  void doUnlight();

  AbstractPlug *_plug;   
  GearGui *_gearGui;  
  QPointF _pos;    
  eStatus _status;
  int _hilightScaling;
  QColor _extrudedRoundBoxColor;

  std::vector<ConnectionItem*> _connectionItems;

  static const int CONNECTION_HANDLE_OFFSETX;
  static const int CONNECTION_HANDLE_OFFSETY;
  static const int PLUG_NAME_NB_CHARS;
  static const int MAX_HILIGHTSCALING;
  static const int ROUNDING_FACTOR;
  static const int PLUGBOX_EXTRUDE;
	
  //fonts
  static const QFont SHORTNAME_FONT;
	
  //colors
  static const QColor EXPOSED_COLOR;  
	

};

#endif
