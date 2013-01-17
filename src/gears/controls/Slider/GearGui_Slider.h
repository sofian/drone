/* GearGui_Slider.h
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

#ifndef GEARGUI_SLIDER_INCLUDED
#define GEARGUI_SLIDER_INCLUDED

#include "GearGui.h"


#include <QGraphicsScene.h>
#include <vector>

class Gear_Slider;

class GearGui_Slider : public GearGui
{
public:
  static const int SLIDER_THICKNESS;    
  static const int SLIDER_SIZEY;
  static const int SLIDER_SIZEX;
  static const int SLIDER_LINE_OFFSETY;
  static const QColor SLIDER_COLOR;
  static const QColor SLIDER_BOX_COLOR;

  GearGui_Slider(Gear_Slider *gear, QGraphicsScene *scene);

  bool mouseEvent(const QPoint& p, Qt::ButtonState button);
  void moveSlider(int sliderPos);

protected:


  void paint(QPainter* painter,const QStyleOptionGraphicsItem *option, QWidget *widget);


};


#endif

