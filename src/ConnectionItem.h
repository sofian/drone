/* ConnectionItem.h
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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


#include <qcanvas.h>
class PlugBox;
class Engine;

class QDomDocument;
class QDomElement;

class ConnectionItem : public QCanvasLine
{
public:
  static const int CANVAS_RTTI_CONNECTION;
  enum eState
  {
    DISCONNECTED, CONNECTING, CONNECTED
  };

  ConnectionItem(Engine *engine, QCanvas *canvas);
  virtual ~ConnectionItem();

  int rtti () const {return CANVAS_RTTI_CONNECTION;};

  void setStartingPlugBox(PlugBox *plugBox);
  void setConnectionLineEndPoint(QPoint const &p);

  PlugBox *sourcePlugBox();
  PlugBox *destPlugBox();

  bool createConnection(PlugBox *plugBox);

  void createConnectionLineOnly(PlugBox *source, PlugBox *dest);

  void hiLight(bool hi);

protected:

//    void updateAreaPoints();
  void drawShape(QPainter &painter);

  void getOrigin(int *x, int *y);
  void getDest(int *x, int *y);

  eState _state;

  bool _hiLighted;
  int _destPointX,_destPointY;

  PlugBox* _sourcePlugBox;
  PlugBox* _destPlugBox;

  Engine *_engine;

  QPen* _pen;

};


#endif
