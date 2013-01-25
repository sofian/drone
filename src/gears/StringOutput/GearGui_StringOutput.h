/* GearGui_StringOutput.h
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

#ifndef GearGui_StringOutput_INCLUDED
#define GearGui_StringOutput_INCLUDED

#include <QGraphicsScene>
#include <QImage>

#include <vector>


#include "GearGui.h"
#include "Gear_StringOutput.h"


class GearGui_StringOutput : public GearGui
{
public:
  GearGui_StringOutput(Gear_StringOutput *gear, QGraphicsScene *scene);
	
void paint(QPainter* painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
	
protected:
  static const int UPDATE_RATE_MS;
	
};

#endif

