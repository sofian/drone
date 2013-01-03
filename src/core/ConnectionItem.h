/* ConnectionItem.h
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

#ifndef CONNECTIONITEM_INCLUDED
#define CONNECTIONITEM_INCLUDED


#include <QGraphicsLineItem>
class PlugBox;
class Engine;

class QDomDocument;
class QDomElement;

class ConnectionItem : public QGraphicsLineItem
{
public:
  enum { Type = UserType + 2 };
  static const int CANVAS_RTTI_CONNECTION;
  enum eState
  {
    DISCONNECTED, CONNECTING, CONNECTED
  };

  ConnectionItem();
  virtual ~ConnectionItem();

  int rtti () const {return CANVAS_RTTI_CONNECTION;};

  void setSourcePlugBox(PlugBox *plugBox);
  void setDestPlugBox(PlugBox *plugBox);

  void setConnectionLineEndPoint(QPointF const &p);

  PlugBox* sourcePlugBox();
  PlugBox* destPlugBox();

  bool ready();
  
  //void createConnectionLineOnly(PlugBox *source, PlugBox *dest);

  void hiLight(bool hi);

  int type() const{return Type;}

protected:

//    void updateAreaPoints();
  void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);

  QPointF getOrigin();
  QPointF getDest();

  eState _state;

  bool _hiLighted;
  QPointF _destPoint;

  PlugBox* _sourcePlugBox;
  PlugBox* _destPlugBox;

  QPen* _pen;

};


#endif
