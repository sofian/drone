/* GearGui_StringOutput.cpp
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



#include "GearGui_StringOutput.h"
#include "Engine.h"


#include <iostream>

const int GearGui_StringOutput::UPDATE_RATE_MS = 83;

GearGui_StringOutput::GearGui_StringOutput(Gear_StringOutput *gear, QGraphicsScene *scene) : 
GearGui(gear, scene, GearGui::BOXNAME_COLOR, 100, 0, UPDATE_RATE_MS)
{            
}

void GearGui_StringOutput::paint(QPainter* painter,const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
  GearGui::paint(painter,option,widget);
	
	int x, y, sizeX, sizeY;      
  getDrawableArea(&x, &y, &sizeX, &sizeY);
	
  painter->setPen(Qt::black);
  painter->setBrush(BOXNAME_COLOR);
  painter->drawRect(x,y,sizeX,sizeY);

  painter->setFont(GearGui::NAME_FONT);
  painter->setPen(Qt::white);
  painter->drawText(x, y, sizeX, sizeY, Qt::AlignHCenter | Qt::AlignVCenter, ((Gear_StringOutput*)_gear)->getString().c_str());
	
}
