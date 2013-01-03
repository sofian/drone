/* GearGui_TouchPad.cpp
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

#include "GearGui_TouchPad.h"
#include "Gear_TouchPad.h"    

#include <qpainter.h>
#include <iostream>
#include <sstream>


const int GearGui_TouchPad::TOUCHPAD_THICKNESS = 5;
const int GearGui_TouchPad::TOUCHPAD_SIZEY = 120;
const int GearGui_TouchPad::TOUCHPAD_SIZEX = 120;
const int GearGui_TouchPad::TOUCHPAD_LINE_OFFSETY = TOUCHPAD_SIZEY / 2;
const QColor GearGui_TouchPad::TOUCHPAD_COLOR(249, 169, 7);
const QColor GearGui_TouchPad::TOUCHPAD_BOX_COLOR(105, 122, 125);

GearGui_TouchPad::GearGui_TouchPad(Gear_TouchPad *gear, QGraphicsScene *scene) : 
  GearGui(gear, scene, GearGui::BOXNAME_COLOR, TOUCHPAD_SIZEX, TOUCHPAD_SIZEY)    
{
}

void GearGui_TouchPad::paint(QPainter* painter,const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget); 
  //make the title bar show the value
  std::ostringstream sstream;
  sstream << "(" << ((Gear_TouchPad*)_gear)->getHorizontalValue() << "," << ((Gear_TouchPad*)_gear)->getVerticalValue() << ")";
  setTitle(sstream.str());

  GearGui::paint(painter,option,widget);    

  int sliderStartX, sliderStartY, sizeX, sizeY;
  getDrawableArea(&sliderStartX, &sliderStartY, &sizeX , &sizeY);

  //convert sliderpos to screen value
  float xlow = _gear->settings().get(Gear_TouchPad::SETTING_HORIZONTAL_LOWERBOUND)->valueFloat();
  float xhi = _gear->settings().get(Gear_TouchPad::SETTING_HORIZONTAL_HIGHERBOUND)->valueFloat();
  float xScreenRatio;
  if (xhi == xlow)
    xScreenRatio = 0.5f;
  else
    xScreenRatio = (((Gear_TouchPad*)_gear)->getHorizontalValue()-xlow) / (xhi-xlow);    
  int xSliderPos = (int) ((float)(xScreenRatio * (float)sizeX));

  //convert sliderpos to screen value
  float ylow = _gear->settings().get(Gear_TouchPad::SETTING_VERTICAL_LOWERBOUND)->valueFloat();
  float yhi = _gear->settings().get(Gear_TouchPad::SETTING_VERTICAL_HIGHERBOUND)->valueFloat();
  float yScreenRatio;
  if (yhi == ylow)
    yScreenRatio = 0.5f;
  else
    yScreenRatio = (((Gear_TouchPad*)_gear)->getVerticalValue()-ylow) / (yhi-ylow);    
  int ySliderPos = (int) ((float)(yScreenRatio * (float)sizeY));
  
  //box                      
  painter->setPen(Qt::black);
  painter->setBrush(TOUCHPAD_BOX_COLOR);
  painter->drawRect(sliderStartX, sliderStartY, sizeX, sizeY);
  //  painter->drawRect(sliderStartX, sliderStartY + TOUCHPAD_LINE_OFFSETY, sizeX, 1);

  //3d
  painter->setPen(Qt::white);
  painter->drawRect(sliderStartX, sliderStartY + sizeY - 1, sizeX, 1);
  painter->drawRect(sliderStartX + sizeX - 1, sliderStartY, 1, sizeY);

  //slider
  painter->setPen(Qt::black);
  //  painter->setBrush(TOUCHPAD_COLOR);
  painter->drawLine(sliderStartX + xSliderPos - TOUCHPAD_THICKNESS, sliderStartY + ySliderPos,
                   sliderStartX + xSliderPos + TOUCHPAD_THICKNESS, sliderStartY + ySliderPos);
  painter->drawLine(sliderStartX + xSliderPos, sliderStartY + ySliderPos - TOUCHPAD_THICKNESS,
                   sliderStartX + xSliderPos, sliderStartY + ySliderPos + TOUCHPAD_THICKNESS);
  ///  painter->drawEllipse(sliderStartX + xSliderPos, sliderStartY + ySliderPos, TOUCHPAD_THICKNESS, TOUCHPAD_THICKNESS);

}

bool GearGui_TouchPad::mouseEvent(const QPoint& p, Qt::ButtonState button)
{
  if (button != Qt::LeftButton)
    return false;

  int sliderStartX, sliderStartY, sizeX, sizeY;
  getDrawableArea(&sliderStartX, &sliderStartY, &sizeX , &sizeY);

  //slider zone ?
  if ((p.y() < sliderStartY)
      || (p.x() > sliderStartX + sizeX - TOUCHPAD_THICKNESS)
      || (p.x() < sliderStartX))
    return false;

  moveTouchPad((int)p.x() - sliderStartX, (int)p.y() - sliderStartY);
  
  return true;
}

void GearGui_TouchPad::moveTouchPad(int xSliderPos, int ySliderPos)
{
  //convert screen sliderpos to value
  int sliderStartX, sliderStartY, sizeX, sizeY;
  getDrawableArea(&sliderStartX, &sliderStartY, &sizeX , &sizeY);

  float xScreenRatio = (float)xSliderPos / (float)(sizeX-TOUCHPAD_THICKNESS);
  float xlow = _gear->settings().get(Gear_TouchPad::SETTING_HORIZONTAL_LOWERBOUND)->valueFloat();
  float xhi = _gear->settings().get(Gear_TouchPad::SETTING_HORIZONTAL_HIGHERBOUND)->valueFloat();
  float xIncAmount = (xhi - xlow) * xScreenRatio;

  float xValue = xlow + xIncAmount;
  //by the constructor we know that it's really a Gear_TouchPad
  ((Gear_TouchPad*)_gear)->setHorizontalValue(xValue);

  float yScreenRatio = (float)ySliderPos / (float)(sizeY-TOUCHPAD_THICKNESS);
  float ylow = _gear->settings().get(Gear_TouchPad::SETTING_VERTICAL_LOWERBOUND)->valueFloat();
  float yhi = _gear->settings().get(Gear_TouchPad::SETTING_VERTICAL_HIGHERBOUND)->valueFloat();
  float yIncAmount = (yhi - ylow) * yScreenRatio;

  float yValue = ylow + yIncAmount;
  //by the constructor we know that it's really a Gear_TouchPad
  ((Gear_TouchPad*)_gear)->setVerticalValue(yValue);

  update();
  scene()->update();   
}



