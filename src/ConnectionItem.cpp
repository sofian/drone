/* ConnectionItem.cpp
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

#include "ConnectionItem.h"
#include "PlugBox.h"
#include "Plug.h"
#include "Gear.h"
#include <qpainter.h>
#include <qdom.h>
#include <iostream>

const int ConnectionItem::CANVAS_RTTI_CONNECTION = 2001;//space odissey


ConnectionItem::ConnectionItem(Engine *engine, QCanvas *canvas) : 
  QCanvasLine(canvas), 
  _state(DISCONNECTED),
  _hiLighted(false),
  _destPointX(0),
  _destPointY(0),
  _sourcePlugBox(NULL),
  _destPlugBox(NULL),
  _engine(engine)

{
  setZ(255);    
  _pen = new QPen(Qt::black, 2, Qt::SolidLine);
  setPen(*_pen);        
  setPoints(0,0,1,1);
}

ConnectionItem::~ConnectionItem()
{        
  //we disconnect but specify to not delete the connectionItem
  //since we are currently destroying it!
  if (_state == CONNECTED)
    _sourcePlugBox->disconnect(this, false);
}

void ConnectionItem::drawShape(QPainter &painter)
{
  int sourceX, sourceY, destX, destY;

  getOrigin(&sourceX, &sourceY);
  getDest(&destX, &destY);

  setPoints(sourceX, sourceY, destX, destY);
  
  QCanvasLine::drawShape(painter);
}

void ConnectionItem::getOrigin(int *x, int *y)
{  
  *x = _sourcePlugBox->connectionHandleX();
  *y = _sourcePlugBox->connectionHandleY();

}

void ConnectionItem::getDest(int *x, int *y)
{
  if (_state == CONNECTING)
  {
    *x = _destPointX;
    *y = _destPointY;
  } else if (_state == CONNECTED)
  {
    *x = _destPlugBox->connectionHandleX(); 
    *y = _destPlugBox->connectionHandleY();
  } else
  {
    *x=0;
    *y=0;
  }
}

PlugBox* ConnectionItem::sourcePlugBox()
{
  return _sourcePlugBox;
}

PlugBox* ConnectionItem::destPlugBox()
{
  return _destPlugBox;
}

void ConnectionItem::setStartingPlugBox(PlugBox *plugBox)
{
  _state = CONNECTING;
  _sourcePlugBox = plugBox;
  _destPointX = plugBox->x();
  _destPointY = plugBox->y();    
  _pen->setColor(_sourcePlugBox->color());
  setPen(*_pen);

  int sourceX, sourceY;
  getOrigin(&sourceX, &sourceY);
  setPoints(sourceX, sourceY, _destPointX, _destPointY);
}

void ConnectionItem::setConnectionLineEndPoint(QPoint const &p)
{
  _destPointX = p.x();
  _destPointY = p.y();
  
  int sourceX, sourceY;
  getOrigin(&sourceX, &sourceY);
  setPoints(sourceX, sourceY, _destPointX, _destPointY);

  update();
  canvas()->update();     
}


bool ConnectionItem::createConnection(PlugBox *plugBox)
{
  if (plugBox==NULL)
    return false;


  if (!_sourcePlugBox->canConnectWith(plugBox))
  {
    _state = DISCONNECTED;
    return false;        
  }


  _sourcePlugBox->connect(plugBox, this);   
  _destPlugBox = plugBox;
  _state = CONNECTED;
  return true;
}

void ConnectionItem::hiLight(bool hi)
{
  //do nothing is hilight status not changed
  if (hi == _hiLighted)
    return;

  _hiLighted=hi;

  if (hi)
  {
    _pen->setColor(Qt::white);
    setPen(*_pen);
  } else
  {
    _pen->setColor(_sourcePlugBox->color());
    setPen(*_pen);
  }

  update();
  canvas()->update();
}

void ConnectionItem::createConnectionLineOnly(PlugBox *source, PlugBox *dest)
{
  _state = CONNECTED;    
  _sourcePlugBox = source;
  _destPlugBox = dest;

  source->assignConnectionOnly(dest, this);

  _pen->setColor(_sourcePlugBox->color());
  setPen(*_pen);    

  int sourceX, sourceY, destX, destY;
  getOrigin(&sourceX, &sourceY);
  getDest(&destX, &destY);
  setPoints(sourceX, sourceY, destX, destY);


  update();
}
