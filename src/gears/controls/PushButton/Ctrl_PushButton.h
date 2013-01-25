/* GearGui_PushButton.h
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

#ifndef GEARGUI_PUSHBUTTON_INCLUDED
#define GEARGUI_PUSHBUTTON_INCLUDED

#include "controlHosts/PushButton/Gear_PushButton.h"

#include <QGraphicsScene>
//Added by qt3to4:
#include <QKeyEvent>
#include <vector>

class Ctrl_PushButton : public Control
{
public:
  static const qreal PUSHBUTTON_SIZEY;
  static const qreal PUSHBUTTON_SIZEX;
  static const QColor PUSHBUTTON_COLOR;
  static const QColor PUSHBUTTON_BOX_COLOR;
  static const QColor PUSHBUTTON_BOX_COLORON;

  Ctrl_PushButton();
  ~Ctrl_PushButton(){};
  
  QString getGearType(){return "Gear:Drone:PushButton";}

protected:
  Gear_PushButton* getGear(){return (static_cast<Gear_PushButton*>(_parentGear));}
  void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
  void mousePressEvent ( QGraphicsSceneMouseEvent * event );
  void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

  bool _pressed;
};


#endif

