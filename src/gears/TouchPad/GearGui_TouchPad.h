/* GearGui_TouchPad.h
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

#ifndef GEARGUI_TOUCHPAD_INCLUDED
#define GEARGUI_TOUCHPAD_INCLUDED

#include "GearGui.h"


#include <qcanvas.h>
#include <vector>

class Gear_TouchPad;

class GearGui_TouchPad : public GearGui
{
public:
  static const int TOUCHPAD_THICKNESS;
  static const int TOUCHPAD_SIZEY;
  static const int TOUCHPAD_SIZEX;
  static const int TOUCHPAD_LINE_OFFSETY;
  static const QColor TOUCHPAD_COLOR;
  static const QColor TOUCHPAD_BOX_COLOR;

  GearGui_TouchPad(Gear_TouchPad *gear, QCanvas *canvas);

  bool mouseEvent(const QPoint& p, Qt::ButtonState button);
  void moveTouchPad(int xSliderPos, int ySliderPos);

protected:


  void drawNormal(QPainter &painter);
  void drawSmall(QPainter &painter);


};


#endif

