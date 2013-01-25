/* GearGui_ListBox.cpp
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

#include "GearGui_ListBox.h"
#include "Gear_ListBox.h"    

#include <qpainter.h>
#include <iostream>
#include <sstream>


const int GearGui_ListBox::SLIDER_THICKNESS = 10;
const int GearGui_ListBox::SLIDER_SIZEY = 20;
const int GearGui_ListBox::SLIDER_SIZEX = 120;
const int GearGui_ListBox::SLIDER_LINE_OFFSETY = SLIDER_SIZEY / 2;
const QColor GearGui_ListBox::SLIDER_COLOR(249, 169, 7);
const QColor GearGui_ListBox::SLIDER_BOX_COLOR(105, 122, 125);

GearGui_ListBox::GearGui_ListBox(Gear_ListBox *gear, QGraphicsScene *scene) : 
GearGui(gear, scene, GearGui::BOXNAME_COLOR, SLIDER_SIZEX, SLIDER_SIZEY)    
{        
}

void GearGui_ListBox::paint(QPainter* painter,const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);    
  //make the title bar show the value
  std::ostringstream sstream;
  QString str = ((Gear_ListBox*)_gear)->getLabel();
  sstream << str;//((Gear_ListBox*)_gear)->getLabel();
  setTitle(sstream.str());

  GearGui::paint(painter,option,widget);    

  int sliderStartX, sliderStartY, sizeX, sizeY;
  getDrawableArea(&sliderStartX, &sliderStartY, &sizeX , &sizeY);

  //convert sliderpos to screen value    
  int hi = _gear->settings().get(Gear_ListBox::SETTING_NELEMS)->valueInt()-1;
  float screenRatio;
  if (hi == 0)
    screenRatio = 0.5f;
  else
    screenRatio = ((Gear_ListBox*)_gear)->getValue() / (float)hi;
  int sliderPos = (int) ((float)(screenRatio * (float)(sizeX-SLIDER_THICKNESS)));

  //box                      
  painter->setPen(Qt::black);
  painter->setBrush(SLIDER_BOX_COLOR);
  painter->drawRect(sliderStartX, sliderStartY, sizeX, sizeY);
  painter->drawRect(sliderStartX, sliderStartY + SLIDER_LINE_OFFSETY, sizeX, 1);

  //3d
  painter->setPen(Qt::white);
  painter->drawRect(sliderStartX, sliderStartY + sizeY - 1, sizeX, 1);
  painter->drawRect(sliderStartX + sizeX - 1, sliderStartY, 1, sizeY);

  //slider
  painter->setPen(Qt::black);
  painter->setBrush(SLIDER_COLOR);
  painter->drawRoundRect(sliderStartX + sliderPos, sliderStartY, SLIDER_THICKNESS, sizeY, 85);

}

bool GearGui_ListBox::mouseEvent(const QPoint& p, Qt::ButtonState button)
{
  if (button != Qt::LeftButton)
    return false;

  int sliderStartX, sliderStartY, sizeX, sizeY;
  getDrawableArea(&sliderStartX, &sliderStartY, &sizeX , &sizeY);

  //slider zone ?
  if ((p.y() < sliderStartY)
      || (p.x() > sliderStartX + sizeX - SLIDER_THICKNESS)
      || (p.x() < sliderStartX))
    return false;

  moveListBox((int)p.x() - (sliderStartX));
  
  return true;
}

void GearGui_ListBox::moveListBox(int sliderPos)
{
  //convert screen sliderpos to value
  int sliderStartX, sliderStartY, sizeX, sizeY;
  getDrawableArea(&sliderStartX, &sliderStartY, &sizeX , &sizeY);

  float screenRatio = (float)sliderPos / (float)(sizeX-SLIDER_THICKNESS);
  int hi = _gear->settings().get(Gear_ListBox::SETTING_NELEMS)->valueInt();
  int value = (int) ((float)hi * screenRatio);

  //by the constructor we know that it's really a Gear_ListBox
  ((Gear_ListBox*)_gear)->setValue(value);

  update();
  scene()->update();   
}



