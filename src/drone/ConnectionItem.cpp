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
#include <QGraphicsScene>
#include <qpainter.h>
#include <qdom.h>
#include <iostream>
#include <QStyleOptionGraphicsItem>

const int ConnectionItem::CANVAS_RTTI_CONNECTION = 2001;//space odissey


ConnectionItem::ConnectionItem() : 
  QGraphicsLineItem(), 
  _state(DISCONNECTED),
  _hiLighted(false),
  _destPoint(QPointF()),
  _sourcePlugBox(NULL),
  _destPlugBox(NULL)

{
  setZValue(255);    
  QVector<qreal> vec;
  vec<<1<<1;
  _pen = new QPen(Qt::black, 2, Qt::SolidLine);
  
  
   QVector<qreal> dashes;
 qreal space = 4;
 dashes << 1 << space << 3 << space << 9 << space
            << 27 << space << 9 << space;
 _pen->setDashPattern(dashes);
 setPen(*_pen);        
  setLine(0,0,1,1);
  setFlags(ItemIsSelectable);
}

ConnectionItem::~ConnectionItem()
{
  delete _pen;
}

void ConnectionItem::paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(widget);

  QPointF source, dest;

  source = getOrigin();
  dest = getDest();

  setLine(QLineF(source, dest));
  //std::cout << sourceX << ":" << sourceY << ":" << destX << ":" << destY << std::endl;
  //_pen->setBrush(QBrush(Qt::Dense2Pattern)); 
  _pen->setStyle(isSelected() ? Qt::DotLine : Qt::SolidLine);
  _pen->setWidth(isSelected() ? 4 : 3);
  setPen(*_pen);

//  std::cerr << (isSelected() ? std::string("sel"):std::string("notsel"))<<std::endl;

  // use this hack to hide default line selection rectangle
  QStyleOptionGraphicsItem *_o = const_cast<QStyleOptionGraphicsItem*>(option);
  _o->state &= ~QStyle::State_Selected;
  
  QGraphicsLineItem::paint(painter,option,widget);
  
}

QPointF ConnectionItem::getOrigin()
{  
  return _sourcePlugBox->connectionHandlePos();
}

QPointF ConnectionItem::getDest()
{
  QPointF dest;
  if (_state == CONNECTING)
  {
    return _destPoint;
  } else if (_state == CONNECTED)
  {
    return _destPlugBox->connectionHandlePos(); 
  } else
  {
    return QPointF();
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

bool ConnectionItem::ready()
{
  return (_sourcePlugBox!=NULL && _destPlugBox!=NULL);
}

void ConnectionItem::setSourcePlugBox(PlugBox *plugBox)
{
  _state = CONNECTING;
  _sourcePlugBox = plugBox;
  _destPoint = plugBox->scenePos();
  _pen->setColor(_sourcePlugBox->color());
  setPen(*_pen);

  setLine(QLineF(getOrigin(),plugBox->scenePos()));
}

void ConnectionItem::setDestPlugBox(PlugBox *plugBox)
{
  if (plugBox==NULL)
  {
    _state = DISCONNECTED;
    return;
  }
  
  _destPlugBox = plugBox;    
  _state = CONNECTED;
}

void ConnectionItem::setConnectionLineEndPoint(QPointF const &p)
{
  _destPoint = p;
  setLine(QLineF(getOrigin(), _destPoint));

  update();
}

void ConnectionItem::hiLight(bool hi)
{
  //do nothing is hilight status not changed
  if (hi == _hiLighted)
    return;

  _hiLighted=hi;

//  if (hi)
//  {
//    _pen->setColor(Qt::white);
//    setPen(*_pen);
//  } else
//  {
//    _pen->setColor(_sourcePlugBox->color());
//    setPen(*_pen);
//  }

  update();
}

/* void ConnectionItem::createConnectionLineOnly(PlugBox *source, PlugBox *dest) */
/* {                                                                             */
/*   _state = CONNECTED;                                                         */
/*   _sourcePlugBox = source;                                                    */
/*   _destPlugBox = dest;                                                        */
/*                                                                               */
/*   source->assignConnectionOnly(dest, this);                                   */
/*                                                                               */
/*   _pen->setColor(_sourcePlugBox->color());                                    */
/*   setPen(*_pen);                                                              */
/*                                                                               */
/*   int sourceX, sourceY, destX, destY;                                         */
/*   getOrigin(&sourceX, &sourceY);                                              */
/*   getDest(&destX, &destY);                                                    */
/*   setPoints(sourceX, sourceY, destX, destY);                                  */
/*                                                                               */
/*                                                                               */
/*   update();                                                                   */
/* }                                                                             */
