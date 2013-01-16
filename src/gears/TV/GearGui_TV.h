/* GearGui_TV.h
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

#ifndef GEARGUI_TV_INCLUDED
#define GEARGUI_TV_INCLUDED

#include <QGraphicsScene.h>
#include <vector>
#include <qimage.h>


#include "Gear_TV.h"


class GearGui_TV : public GearGui
{
public:

  GearGui_TV(Gear_TV *gear, QGraphicsScene *scene);


protected:
  QImage _videoFrame;    

  void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);




private:

  static const QColor NO_SIGNAL_COLOR;
  static const int TV_SIZEX;
  static const int TV_SIZEY;
  static const int UPDATE_RATE_MS;

  int _currentSizeX;
  int _currentSizeY;



};

#endif

