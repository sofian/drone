/* GearGui_PushButton.h
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#include "GearGui.h"


#include <qcanvas.h>
#include <vector>

class Gear_PushButton;

class GearGui_PushButton : public GearGui
{
public:
  static const int PUSHBUTTON_SIZEY;
  static const int PUSHBUTTON_SIZEX;
  static const QColor PUSHBUTTON_COLOR;
  static const QColor PUSHBUTTON_BOX_COLOR;
  static const QColor PUSHBUTTON_BOX_COLORON;

  GearGui_PushButton(Gear_PushButton *gear, QCanvas *canvas);

  bool mouseEvent(const QPoint& p, Qt::ButtonState button);
  bool keyEvent(QKeyEvent *e);
  
protected:

  void drawShape(QPainter &painter);


};


#endif

