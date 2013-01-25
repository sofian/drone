/* GearGui_AudioPushButton.cpp
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

#include "GearGui_AudioPushButton.h"
#include "Gear_AudioPushButton.h"    

#include <qpainter.h>
#include <iostream>
#include <sstream>


const int GearGui_AudioPushButton::PUSHBUTTON_SIZEY = 20;
const int GearGui_AudioPushButton::PUSHBUTTON_SIZEX = 60;
const QColor GearGui_AudioPushButton::PUSHBUTTON_COLOR(249, 169, 7);
const QColor GearGui_AudioPushButton::PUSHBUTTON_BOX_COLOR(105, 122, 125);
const QColor GearGui_AudioPushButton::PUSHBUTTON_BOX_COLORON(185, 115, 25);

GearGui_AudioPushButton::GearGui_AudioPushButton(Gear_AudioPushButton *gear, QGraphicsScene *scene) : 
GearGui(gear, scene, GearGui::BOXNAME_COLOR, PUSHBUTTON_SIZEX, PUSHBUTTON_SIZEY)    
{        
}

void GearGui_AudioPushButton::paint(QPainter* painter,const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);   
  //make the title bar show the value
  std::ostringstream sstream;
  sstream << ((Gear_AudioPushButton*)_gear)->getValue();
  setTitle(sstream.str());

  GearGui::paint(painter,option,widget);    

  int AudioPushButtonStartX, AudioPushButtonStartY, sizeX, sizeY;
  getDrawableArea(&AudioPushButtonStartX, &AudioPushButtonStartY, &sizeX , &sizeY);

  QColor border1;
  QColor border2;
  QColor box;


  if(((Gear_AudioPushButton*)_gear)->getState()==ON)
  {
    border1=Qt::black;
    border2=Qt::white;
    box = PUSHBUTTON_BOX_COLORON;
  }
  else
  {
    border2=Qt::black;
    border1=Qt::white;
    box = PUSHBUTTON_BOX_COLOR;
  }


  //box                      
  painter->setPen(border1);
  painter->setBrush(box);
  painter->drawRect(AudioPushButtonStartX, AudioPushButtonStartY, sizeX, sizeY);

  //3d
  painter->setPen(border2);
  painter->drawRect(AudioPushButtonStartX, AudioPushButtonStartY + sizeY - 1, sizeX, 1);
  painter->drawRect(AudioPushButtonStartX + sizeX - 1, AudioPushButtonStartY, 1, sizeY);

}

bool GearGui_AudioPushButton::mouseEvent(const QPoint& p, Qt::ButtonState button)
{
  bool ret=false;

  // Pushbutton zone ?
if (((Gear_AudioPushButton*)_gear)->getState()==ON && button == Qt::NoButton)
  {
    ((Gear_AudioPushButton*)_gear)->setState(OFF);
    ret=true;
  }
  else if (((Gear_AudioPushButton*)_gear)->getState()==OFF && button == Qt::LeftButton)
  {
    static int i=0;

    int sliderStartX, sliderStartY, sizeX, sizeY;
    getDrawableArea(&sliderStartX, &sliderStartY, &sizeX , &sizeY);

    if (p.y() < sliderStartY || p.y() > sliderStartY + sizeY ||
        p.x() < sliderStartX || p.x() > sliderStartX + sizeX)
      ret = false;
    else
    {
      NOTICE("Pushbutton: %d",i++);
      ((Gear_AudioPushButton*)_gear)->setState(ON);
      ret=true;
    }
  }
  update();
  scene()->update();   
  return ret;
}

bool GearGui_AudioPushButton::keyEvent(QKeyEvent *e)
{
  std::cerr<<e->ascii()<<" "<<e->stateAfter()<<std::endl;
  return false;
}
  
