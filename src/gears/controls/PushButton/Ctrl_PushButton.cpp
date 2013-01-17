/* GearGui_PushButton.cpp
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

#include "Ctrl_PushButton.h"
#include "Gear_PushButton.h"    

#include <qpainter.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <iostream>
#include <sstream>


const int Ctrl_PushButton::PUSHBUTTON_SIZEY = 20;
const int Ctrl_PushButton::PUSHBUTTON_SIZEX = 60;
const QColor Ctrl_PushButton::PUSHBUTTON_COLOR(249, 169, 7);
const QColor Ctrl_PushButton::PUSHBUTTON_BOX_COLOR(105, 122, 125);
const QColor Ctrl_PushButton::PUSHBUTTON_BOX_COLORON(185, 115, 25);

Ctrl_PushButton::Ctrl_PushButton(GearControlHost *gear) : 
Control()    
{       
  resize(QRectF(PUSH_BUTTON_SIZEX,PUSH_BUTTON_SIZEY));
  
}

void Ctrl_PushButton::paint(QPainter* painter,const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(widget);
  //make the title bar show the value
  painter.setPen( Qt::black );
    
  QPoint point = QPoint( 10, 20 );
  painter.drawText( point, "You can draw text from a point..." );
  
  return;
  /*
  std::ostringstream sstream;
  sstream << ((Gear_PushButton*)_gear)->getValue();
  setTitle(sstream.str());

  Ctrl::paint(painter,option,widget);    

  int PushButtonStartX, PushButtonStartY, sizeX, sizeY;
  getDrawableArea(&PushButtonStartX, &PushButtonStartY, &sizeX , &sizeY);

  QColor border1;
  QColor border2;
  QColor box;


  if(((Gear_PushButton*)_gear)->getState()==ON)
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
  painter->drawRect(PushButtonStartX, PushButtonStartY, sizeX, sizeY);

  //3d
  painter->setPen(border2);
  painter->drawRect(PushButtonStartX, PushButtonStartY + sizeY - 1, sizeX, 1);
  painter->drawRect(PushButtonStartX + sizeX - 1, PushButtonStartY, 1, sizeY);
*/
}

bool Ctrl_PushButton::mouseEvent(const QPoint& p, Qt::ButtonState button)
{
  bool ret=false;

  // Pushbutton zone ?
  if (((Gear_PushButton*)_gear)->getState()==ON && button == Qt::NoButton)
  {
    ((Gear_PushButton*)_gear)->setState(OFF);
    ret=true;
  }
  else if (((Gear_PushButton*)_gear)->getState()==OFF && button == Qt::LeftButton)
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
      ((Gear_PushButton*)_gear)->setState(ON);
      ret=true;
    }
  }
  update();
  scene()->update();   
  return ret;
}

bool Ctrl_PushButton::keyEvent(QKeyEvent *e)
{
  std::cerr<<e->ascii()<<" "<<e->stateAfter()<<std::endl;
  return false;
}
  
