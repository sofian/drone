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

const int PlugBox::PLUGBOX_SIZE = 10;
const int PlugBox::CONNECTION_HANDLE_OFFSETX = PLUGBOX_SIZE - 2;
const int PlugBox::CONNECTION_HANDLE_OFFSETY = PLUGBOX_SIZE / 2;

#if defined(Q_OS_MACX)
const QFont PlugBox::SHORTNAME_FONT(QFont("Verdana", 9, QFont::Normal));
#else
const QFont PlugBox::SHORTNAME_FONT(QFont("Verdana", 8, QFont::Normal));
#endif

const int PlugBox::MAX_HILIGHTSCALING = 2;
const int PlugBox::PLUG_NAME_NB_CHARS = 6;
const int PlugBox::ROUNDING_FACTOR=100;


const QColor PlugBox::EXPOSED_COLOR(196,8,8);

PlugBox::PlugBox(AbstractPlug *plug, GearGui* gearGui) : 
_plug(plug), 
_gearGui(gearGui),     
_x(0), 
_y(0), 
_status(NORMAL), 
_hilightScaling(0)
{
}

PlugBox::~PlugBox()
{
  disconnectAll();
}

QColor PlugBox::color()
{
  return _plug->abstractType()->color();
}

void PlugBox::draw(int x, int y, int gearSizeX, QPainter &painter)
{    
  int halfGearSizeX = gearSizeX / 2;    
  _x = x;
  _y = y;

	painter.setPen(Qt::NoPen);

	//set color of the round box according to exposition
	if (_plug->exposed())
		painter.setBrush(EXPOSED_COLOR);
	else
		painter.setBrush(GearGui::BOX_COLOR);
		
	//the round box around the plug
	if (_plug->inOut() == IN)
		painter.drawRoundRect(_x - MAX_HILIGHTSCALING*2, _y - MAX_HILIGHTSCALING, PLUGBOX_SIZE + MAX_HILIGHTSCALING*2, PLUGBOX_SIZE + MAX_HILIGHTSCALING*2, ROUNDING_FACTOR, ROUNDING_FACTOR);    
	else
		painter.drawRoundRect(_x, _y - MAX_HILIGHTSCALING, PLUGBOX_SIZE + MAX_HILIGHTSCALING*2, PLUGBOX_SIZE + MAX_HILIGHTSCALING*2, ROUNDING_FACTOR, ROUNDING_FACTOR);		
			
  painter.setPen(Qt::black);
	painter.setBrush(color());
	
	//the plugbox
  if (_plug->inOut() == IN)
    painter.drawRoundRect(_x, _y - _hilightScaling, PLUGBOX_SIZE + _hilightScaling*2, PLUGBOX_SIZE + _hilightScaling*2, ROUNDING_FACTOR, ROUNDING_FACTOR);
  else
    painter.drawRoundRect(_x - _hilightScaling*2, _y - _hilightScaling, PLUGBOX_SIZE + _hilightScaling*2, PLUGBOX_SIZE + _hilightScaling*2, ROUNDING_FACTOR, ROUNDING_FACTOR);    
  
  painter.setFont(SHORTNAME_FONT);  
	
  if (_status==HILIGHTED)
    painter.setPen(Qt::blue);
  else
    painter.setPen(Qt::black);

  //align text left or right if In or Out
  if (_plug->inOut() == IN)
    painter.drawText(_x + PLUGBOX_SIZE + 5, _y - 4, halfGearSizeX, PLUGBOX_SIZE + 8, Qt::AlignLeft | Qt::AlignVCenter, _plug->shortName(PLUG_NAME_NB_CHARS).c_str());
	else
    painter.drawText(_x - halfGearSizeX, _y - 4, halfGearSizeX - 5, PLUGBOX_SIZE + 8, Qt::AlignRight | Qt::AlignVCenter, _plug->shortName(PLUG_NAME_NB_CHARS).c_str());

}

bool PlugBox::hitted(int x, int y)
{
  return((x > _x) && (x < _x+PLUGBOX_SIZE) && (y > _y) && (y < _y+PLUGBOX_SIZE));
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
  ConnectionItem *connectionItem = new ConnectionItem(_gearGui->canvas());
  connectionItem->setSourcePlugBox(this);
  connectionItem->setDestPlugBox(plugBox);
  connectionItem->show();

  //give the new connection item to both plugBox
  _connectionItems.push_back(connectionItem);
  plugBox->_connectionItems.push_back(connectionItem);

  return true;
}

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

int PlugBox::connectionHandleX()
{
  if (_plug->inOut() == IN)
  {
    return x() + 2;
  } else
  {
    return x() + CONNECTION_HANDLE_OFFSETX;
  }
}

int PlugBox::connectionHandleY()
{
  return y() + CONNECTION_HANDLE_OFFSETY;
}


