/* PlugBox.cpp
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

#include "PlugBox.h"
#include "GearGui.h"
#include "Plug.h"
#include "ConnectionItem.h"

#include <iostream>
#include <algorithm>
#include <assert.h>
#include <QGraphicsScene>
#include "GearGui.h"

const int PlugBox::PLUGBOX_RADIUS = 8;
const int PlugBox::PLUGBOX_INBETWEEN_SPACE = 3;
const int PlugBox::CONNECTION_HANDLE_OFFSETX = PLUGBOX_RADIUS - 2;
const int PlugBox::CONNECTION_HANDLE_OFFSETY = PLUGBOX_RADIUS / 2;

#if defined(Q_OS_MACX)
const QFont PlugBox::SHORTNAME_FONT(QFont("Roboto", 9, QFont::Normal));
#else
const QFont PlugBox::SHORTNAME_FONT(QFont("Roboto", 9, QFont::Normal));
#endif

const int PlugBox::PLUGBOX_EXTRUDE = 4;
const int PlugBox::MAX_HILIGHTSCALING = 1;
const int PlugBox::PLUG_NAME_NB_CHARS = 6;
const int PlugBox::ROUNDING_FACTOR=150;

const QColor PlugBox::EXPOSED_COLOR(196,8,8);

PlugBox::PlugBox(AbstractPlug *plug, GearGui* gearGui) : 
_plug(plug), 
_gearGui(gearGui),     
_pos(QPointF()), 
_status(NORMAL), 
_hilightScaling(0)
{
}

PlugBox::~PlugBox()
{
  disconnectAll();
}

qreal PlugBox::plugNameWidth(std::string name)
{
  QFontMetrics fm(SHORTNAME_FONT);
  qreal val= fm.width(name.c_str()) + PLUGBOX_RADIUS + PLUGBOX_INBETWEEN_SPACE;
  return val;
}

// returns the color associated with this plug's type 
QColor PlugBox::color()
{
  return _plug->abstractType()->color();
}

// Draws a bigger yellow colored circle to make the selection emboss on plugs
void PlugBox::drawSelected(QPainter *painter)
{    
  painter->setBrush(GearGui::SELECT_OUTLINE_TOP);
  if (_plug->inOut() == IN)    
    painter->drawPie(_pos.x() - 5, _pos.y() - 6, 20, 20, 1440, 2880);
    painter->drawPie(_pos.x() - 7, _pos.y() - 6, 20, 20, 1440, -2880);
}   

// Draws the actual plug with corresponding label
void PlugBox::draw(QPainter *painter)
{ 
  int halfGearSizeX = _gearGui->sizeX() / 2;    
  painter->setPen(QPen(Qt::black,1));
  QColor cay=_gearGui->colorAtY(_pos.y());
  //set color of the round box according to exposition
  
  if (_plug->exposed())
  {
    _extrudedRoundBoxColor = EXPOSED_COLOR;
  }
  else
    
      _extrudedRoundBoxColor = cay;
//    
  //the round box around the plug
  if (_plug->inOut() == IN)    
  {
    painter->setBrush(_extrudedRoundBoxColor);
    painter->drawPie(_pos.x() - 3, _pos.y() - 4, 16, 16, 1960, 1840);
    painter->setPen(Qt::NoPen);
    painter->setBrush(cay);
    painter->drawRect(QRectF(_pos.x()+0.5, _pos.y()-4, 2.5, 15.5));
    
  }
  else
  {
    painter->setBrush(_extrudedRoundBoxColor);
    painter->drawPie(_pos.x() - 5, _pos.y() - 4, 16, 16, 920, -1840);
    painter->setPen(Qt::NoPen);
    painter->setBrush(cay);
    painter->drawRect(QRectF(_pos.x()+5, _pos.y()-4, 2.5, 15.5));
  }
   
      
  painter->setPen(Qt::black);
  painter->setBrush(color());
  
  //the plugbox
  painter->drawEllipse(_pos.x() - _hilightScaling, _pos.y() - _hilightScaling, PLUGBOX_RADIUS + _hilightScaling*2, PLUGBOX_RADIUS + _hilightScaling*2);
  
  painter->setFont(SHORTNAME_FONT);  
  
  if (_status==HILIGHTED)
    painter->setPen(Qt::blue);
  else
    painter->setPen(Qt::white);

  // if gear's layoutmode is normal, then display plugnames
  if (_gearGui->getLayoutMode() == GearGui::normal)
  {

    //align text left or right if In or Out
    if (_plug->inOut() == IN)
      painter->drawText(_pos.x() + PLUGBOX_RADIUS + PLUGBOX_INBETWEEN_SPACE, _pos.y() - 4, halfGearSizeX, PLUGBOX_RADIUS + 8, Qt::AlignLeft | Qt::AlignVCenter, _plug->shortName(PLUG_NAME_NB_CHARS).c_str());
    else
      painter->drawText(_pos.x() - halfGearSizeX, _pos.y() - 4, halfGearSizeX - PLUGBOX_INBETWEEN_SPACE, PLUGBOX_RADIUS + 8, Qt::AlignRight | Qt::AlignVCenter, _plug->shortName(PLUG_NAME_NB_CHARS).c_str());
  }
}

bool PlugBox::hit(QPointF pos)
{
  int nameWidth=0, hackyYOffset=2;
  // weird Y offset fine tuning.. why ?
  pos.setY(pos.y() - 2);
  if (_gearGui->getLayoutMode() == GearGui::normal)
    nameWidth += PlugBox::plugNameWidth(_plug->name());

  if (_plug->inOut() == IN)
    return ((pos.x() > _pos.x()) && (pos.x() < _pos.x() + PLUGBOX_RADIUS + nameWidth) && (pos.y() > _pos.y()) && (pos.y() < _pos.y() + PLUGBOX_RADIUS));
  else 
    return((pos.x() > _pos.x()-nameWidth) && (pos.x() < _pos.x() + PLUGBOX_RADIUS) && (pos.y() > _pos.y()) && (pos.y() < _pos.y()+PLUGBOX_RADIUS));
}

void PlugBox::hilight(bool hiLight)
{
  switch (_status)
  {
  case NORMAL:
    if (hiLight)
    {
      _status = HILIGHTED;
      doHilight();
    }
    break;
  case HILIGHTED:
    if (!hiLight)
    {
      _status = NORMAL;
      doUnlight();
    }
    break;
  }                
}

void PlugBox::doHilight()
{    
  _hilightScaling = MAX_HILIGHTSCALING;
}

void PlugBox::doUnlight()
{ 
  _hilightScaling = 0;  
}

bool PlugBox::canConnectWith(PlugBox *plugBox)
{
  if (plugBox==NULL)
    return false;

  //we will only perform type checking, because we dont
  //we want the test to refuse multiple connection per inputs.
  //multiple connection per inputs will be handled in
  //SchemaGui::connect where we will disconnect before creating the
  //new connection
  return _plug->canConnect(plugBox->plug(), true);        

}

bool PlugBox::connect(PlugBox *plugBox)
{
  if (!plugBox)
    return false;

  //create a connection item to represent this connection in the schemaGui
  ConnectionItem *connectionItem = new ConnectionItem();
  connectionItem->setSourcePlugBox(this);
  connectionItem->setDestPlugBox(plugBox);
  connectionItem->show();
  _gearGui->scene()->addItem(connectionItem);

  //give the new connection item to both plugBox
  _connectionItems.push_back(connectionItem);
  plugBox->_connectionItems.push_back(connectionItem);

  return true;
}

QPointF PlugBox::scenePos()
{
  return _gearGui->mapToScene(_pos);
};

void PlugBox::disconnect(PlugBox *plugBox)
{  
  //find the corresponding connectionItem
  ConnectionItem *connectionItem=0;
  for (std::vector<ConnectionItem*>::iterator it=_connectionItems.begin(); it!=_connectionItems.end();++it)
  {
    if ( (((*it)->sourcePlugBox()==this) && ((*it)->destPlugBox()==plugBox)) ||
        (((*it)->sourcePlugBox()==plugBox) && ((*it)->destPlugBox()==this)))
    {
      connectionItem=(*it);
      break;
    }
  }
  
  //we are not connected to this plugBox
  //return
  if (!connectionItem)
    return;
  
  //remove connectionItems from both plugboxes
  std::vector<ConnectionItem*> *srcConnectionItems = &(_connectionItems);
  std::vector<ConnectionItem*> *dstConnectionItems = &(plugBox->_connectionItems);

  srcConnectionItems->erase(std::remove(srcConnectionItems->begin(), srcConnectionItems->end(), connectionItem), srcConnectionItems->end());
  dstConnectionItems->erase(std::remove(dstConnectionItems->begin(), dstConnectionItems->end(), connectionItem), dstConnectionItems->end());

  //delete the connectionItem
  delete connectionItem;
}

void PlugBox::disconnectAll()
{
  ConnectionItem *connectionItem;

  while (!_connectionItems.empty())
  {
    connectionItem = _connectionItems.back();        
        
    //by disconnection we will empty the _connectionItems vector
    connectionItem->sourcePlugBox()->disconnect(connectionItem->destPlugBox());    
  }
}

QPointF PlugBox::connectionHandlePos()
{
  QPointF pos(scenePos());
  if (_plug->inOut() == IN)
  {
    return QPointF(pos.x() + 2, pos.y() + CONNECTION_HANDLE_OFFSETY);
  } else
  {
    return QPointF(pos.x() + CONNECTION_HANDLE_OFFSETX, pos.y() + CONNECTION_HANDLE_OFFSETY);
  }
}
