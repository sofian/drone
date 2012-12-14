/* GearGui_TV.cpp
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

#include <qpainter.h>



#include "GearGui_TV.h"
#include "Engine.h"




#include <iostream>

const int GearGui_TV::TV_SIZEX = 125;
const int GearGui_TV::TV_SIZEY = 125;
const int GearGui_TV::UPDATE_RATE_MS = 83;
const QColor GearGui_TV::NO_SIGNAL_COLOR(2, 128, 255);

GearGui_TV::GearGui_TV(Gear_TV *gear, Q3Canvas *canvas) : 
GearGui(gear, canvas, GearGui::BOXNAME_COLOR, TV_SIZEX, TV_SIZEY, UPDATE_RATE_MS),
_currentSizeX(0),
_currentSizeY(0)
{            
}

void GearGui_TV::drawShape(QPainter &painter)
{
  GearGui::drawShape(painter);

  const Array2DType<RGBA> *image;
  const RGBA *data;
  bool noImage=false;

  int x, y, sizeX, sizeY;      
  getDrawableArea(&x, &y, &sizeX, &sizeY);

  //3d
  painter.setPen(Qt::white);
  painter.drawRect(x, y + sizeY + 1, sizeX, 1);
  painter.drawRect(x + sizeX + 1, y, 1, sizeY);

  if (((Gear_TV*)_gear)->VIDEO_IN()->connected())
  {
    image = ((Gear_TV*)_gear)->VIDEO_IN()->type();
    data = image->data();

    if (((int)image->width() != _currentSizeX) || ((int)image->height() != _currentSizeY))
    {
      _videoFrame.create(image->width(), image->height(), 32);
      _currentSizeX = image->width();
      _currentSizeY = image->height();
    }

    unsigned char *bits = _videoFrame.bits();

    int iterSizeX = image->width();
    int iterSizeY = image->height();

    for (int y1=0;y1<iterSizeY;++y1)
      for (int x1=0;x1<iterSizeX;++x1)
      {
        (*(QRgb*)bits) = qRgb(data->r, data->g, data->b); 
        bits+=4;
        data++;
      }
    
    if(_currentSizeX && _currentSizeY)
      painter.drawImage(x, y, _videoFrame.scaled(sizeX, sizeY));
    else noImage=true;
  }
  else noImage=true;

  if(noImage)
  {
    painter.setPen(Qt::black);
    painter.setBrush(NO_SIGNAL_COLOR);
    painter.drawRect(x, y, sizeX, sizeY);
  }

}


