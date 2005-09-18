/* GearGui.cpp
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

#include "GearGui.h"
#include "Gear.h"   
#include "Engine.h"
#include "PlugBox.h"

#include <iostream>
#include <sstream>

#include <qpainter.h>
#include <qdom.h>

const int GearGui::CANVAS_RTTI_GEAR = 2000;
const int GearGui::DEFAULT_SIZEX = 125;

const int GearGui::NAME_SIZEY = 20;
const int GearGui::PLUGBOXES_NOMINAL_INTERVAL = 16;
const int GearGui::PLUGBOXES_STARTING_OFFSET = PLUGBOXES_NOMINAL_INTERVAL / 2;
const int GearGui::SHADOW_OFFSET = 4;
const int GearGui::RENDERING_OFFSET = 4;

const QColor GearGui::BOX_COLOR(207,207,209);
const QColor GearGui::SELECTED_BOX_COLOR(255,255,255);
const QColor GearGui::SHADOW_COLOR(87,102,125);
const QColor GearGui::BOXNAME_COLOR(0,31,68);


#if defined(Q_OS_MACX)
const QFont GearGui::NAME_FONT("Verdana", 12, QFont::Bold);
#else
const QFont GearGui::NAME_FONT("Verdana", 9, QFont::Bold);
#endif

GearGui::GearGui(Gear *pgear, QCanvas *canvas, QColor color, int sizeX, int sizeY, int updateRate) :
QCanvasRectangle(canvas),
QObject(),
_gear(pgear),
_selected(false),
_sizeX(sizeX),
_sizeY(sizeY),
_inputsInterval(0),
_outputsInterval(0),
_boxNameColor(color)
{

  if (updateRate>=0)
    startTimer(updateRate);

  refresh();
  
}

GearGui::~GearGui()
{
  //delete all the plugBoxes
  //plugboxes take care of deleting connectionItems when deleted
  //Everything is taken care of ! :)
  removeAllPlugBoxes();
}

int GearGui::renderingStartX()
{
	return (int)x() + RENDERING_OFFSET;
}

int GearGui::renderingStartY()
{
	return (int)y() + RENDERING_OFFSET;
}

//!create plugBoxes from the parent gear plugs
void GearGui::refresh()
{

  //delete and remove inputplugboxes that we dont have anymore
  //we also erase plugs in inputs that we already have
  std::list<AbstractPlug*> inputs;
  _gear->getInputs(inputs);
  
/*  std::vector<std::vector<PlugBox*>::iterator> inputsToRemove;
  for (std::vector<PlugBox*>::iterator plugBoxit = _inputPlugBoxes.begin(); plugBoxit != _inputPlugBoxes.end(); ++plugBoxit)
  {
    bool found=false;
    std::list<AbstractPlug*>::iterator it;
    for (it = inputs.begin(); it != inputs.end(); ++it)
    {
        if ((*plugBoxit)->plug() == (*it))
        {
          found=true;
          inputs.erase(it);
          break;
        }
    }
    
    if (!found)
    {
      inputsToRemove.push_back(plugBoxit);
      delete (*it);
    }
  }

  for (std::vector<std::vector<PlugBox*>::iterator>::iterator it = inputsToRemove.begin(); it != inputsToRemove.end(); ++it)
    _inputPlugBoxes.erase(*it);
*/
  
  //delete and remove output plugboxes that we dont have anymore
  //we also erase plugs in outputs that we already have
  std::list<AbstractPlug*> outputs;
  _gear->getOutputs(outputs);
  
/*  std::vector<std::vector<PlugBox*>::iterator> outputsToRemove;
  for (std::vector<PlugBox*>::iterator plugBoxit = _outputPlugBoxes.begin(); plugBoxit != _outputPlugBoxes.end(); ++plugBoxit)
  {
    bool found=false;
    std::list<AbstractPlug*>::iterator it;
    for (it = outputs.begin(); it != outputs.end(); ++it)
    {
      if ((*plugBoxit)->plug() == (*it))
      {
        found=true;
        outputs.erase(it);
        break;
      }
    }	
    
    if (!found)
    {
      outputsToRemove.push_back(plugBoxit);
      delete (*it);
    }
  }

  for (std::vector<std::vector<PlugBox*>::iterator>::iterator it = outputsToRemove.begin(); it != outputsToRemove.end(); ++it)
    _outputPlugBoxes.erase(*it);
  
  */
  
  removeAllPlugBoxes();
  
  //now create missing plugboxes
  PlugBox *plugBox;
  for (std::list<AbstractPlug*>::iterator it = inputs.begin(); it != inputs.end(); ++it)
  {
    plugBox = new PlugBox(*it, this);
    _plugBoxes.push_back(plugBox);
    _inputPlugBoxes.push_back(plugBox);
  }
    
  for (std::list<AbstractPlug*>::iterator it = outputs.begin(); it != outputs.end(); ++it)
  {
    plugBox = new PlugBox(*it, this);
    _plugBoxes.push_back(plugBox);
    _outputPlugBoxes.push_back(plugBox);
  }
  
  
  //calculate _sizeY for plugboxes fitting
  int maxPlugs = _inputPlugBoxes.size() > _outputPlugBoxes.size() ? _inputPlugBoxes.size() : _outputPlugBoxes.size();
  
  int newSizeY = ((maxPlugs+1) * PLUGBOXES_NOMINAL_INTERVAL) + NAME_SIZEY;
  
  //fit to desired sizeY
  if (newSizeY > _sizeY)
    _sizeY = newSizeY;
  
  if (_inputPlugBoxes.size())
    _inputsInterval = (_sizeY - NAME_SIZEY) / (_inputPlugBoxes.size()+1);
  else
    _inputsInterval = 0;
  
  if (_outputPlugBoxes.size())
    _outputsInterval = (_sizeY - NAME_SIZEY) / (_outputPlugBoxes.size()+1);
  else
    _outputsInterval = 0;
  
  //now set the size for this QCanvasItem
  //we want a larger rect for shadow and rendering offset
  //since plugs get out of the gear rect
  setSize(_sizeX + SHADOW_OFFSET + RENDERING_OFFSET, _sizeY + SHADOW_OFFSET + RENDERING_OFFSET);
  reDraw();
}

void GearGui::removeAllPlugBoxes()
{
  //delete all the plugBoxes
  //plugboxes take care of deleting connectionItems when deleted
  //Everything is taken care of ! :)
  for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)
    delete (*it);
  _plugBoxes.clear();
  _inputPlugBoxes.clear();
  _outputPlugBoxes.clear();
}

void GearGui::getDrawableArea(int *ox, int *oy, int *sizeX, int *sizeY)
{

  *ox = (int)renderingStartX() + PlugBox::PLUGBOX_SIZE + 4;
  *oy = (int)renderingStartY() + GearGui::NAME_SIZEY + 4;
  *sizeX = _sizeX - (PlugBox::PLUGBOX_SIZE+4)*2;
  *sizeY = _sizeY - GearGui::NAME_SIZEY - 10;            
}

bool GearGui::titleBarHitted(const QPoint& p)
{
  return(p.y() < y() + NAME_SIZEY);    
}

void GearGui::drawPlugBoxes(QPainter &painter)
{
  int startX = (int)renderingStartX();
  int startY = (int)renderingStartY() + NAME_SIZEY;
  int by=0;
  int bx=0;

  if (_inputPlugBoxes.size())
  {
    by=startY + _inputsInterval - PLUGBOXES_STARTING_OFFSET;
    for (std::vector<PlugBox*>::iterator it = _inputPlugBoxes.begin(); it != _inputPlugBoxes.end(); ++it)
    {
      (*it)->draw(startX, by, _sizeX, painter);
      by+=_inputsInterval;
    }
  }

  if (_outputPlugBoxes.size())
  {
    by=startY + _outputsInterval - PLUGBOXES_STARTING_OFFSET;
    bx=startX + _sizeX - PlugBox::PLUGBOX_SIZE;
    for (std::vector<PlugBox*>::iterator it = _outputPlugBoxes.begin(); it != _outputPlugBoxes.end(); ++it)
    {
      (*it)->draw(bx, by, _sizeX, painter);
      by+=_outputsInterval;
    }
  }

}

void GearGui::drawShape(QPainter &painter)
{
  int startX = (int)renderingStartX();
  int startY = (int)renderingStartY();


  //shadow
  painter.setPen(Qt::NoPen);
  painter.setBrush(SHADOW_COLOR);
  painter.drawRoundRect(startX + SHADOW_OFFSET, startY + SHADOW_OFFSET, _sizeX, _sizeY);

  //box
  painter.setPen(Qt::black);
  painter.setBrush(_selected?SELECTED_BOX_COLOR : BOX_COLOR);
  painter.drawRoundRect(startX, startY, _sizeX, _sizeY);

  //name
  painter.setPen(Qt::black);  
  painter.setBrush(_boxNameColor.light(_selected?200:100));

  painter.drawRect(startX, startY, _sizeX, NAME_SIZEY);
  painter.setFont(NAME_FONT);
  painter.setPen(Qt::white);
  
  //title default to gear name if not explicitly setted
  std::string title=_title;
  if (title.empty())
    title=gear()->name();
    
  painter.drawText(startX, startY, _sizeX, NAME_SIZEY, Qt::AlignHCenter | Qt::AlignVCenter, title.c_str());

  //plugs
  drawPlugBoxes(painter);

}

PlugBox* GearGui::plugHitted(const QPoint &p)
{
  for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)
  {
    if ((*it)->hitted(p.x(), p.y()))
      return(*it);
  }

  return NULL;
}

void GearGui::performPlugHighligthing(const QPoint &p)
{
  PlugBox *plugbox;

  
  for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)
  {
    plugbox = *it;

    if (plugbox->hitted(p.x(), p.y()))
      plugbox->hilight(true);
    else
      plugbox->hilight(false);
  }

  //we need to repaint ourself
  reDraw();
}

void GearGui::performPlugHighligthing(PlugBox *plugBox)
{
  for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)  
    if ((*it)!=plugBox)    
      (*it)->hilight(false);
  
  if (plugBox)
    plugBox->hilight(true);
  
reDraw();
}

void GearGui::unHilightAllPlugBoxes()
{
  for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)
    (*it)->hilight(false);

  reDraw();
}

void GearGui::save(QDomDocument &doc, QDomElement &gearElem)                        
{                                                                                                                       
  QDomAttr gearPosX = doc.createAttribute("PosX");
  std::ostringstream strX;
  strX << x();
  gearPosX.setValue(strX.str().c_str());
  gearElem.setAttributeNode(gearPosX);

  QDomAttr gearPosY = doc.createAttribute("PosY");
  std::ostringstream strY;
  strY << y();
  gearPosY.setValue(strY.str().c_str());
  gearElem.setAttributeNode(gearPosY);

}                                                                                                                       

void GearGui::load(QDomElement &gearElem)                        
{
  setX(gearElem.attribute("PosX","").toInt());
  setY(gearElem.attribute("PosY","").toInt());
}


PlugBox* GearGui::getInputPlugBox(std::string name) const
{
  for (std::vector<PlugBox*>::const_iterator it = _inputPlugBoxes.begin();it!=_inputPlugBoxes.end();++it)
  {
    if ((*it)->plug()->name() == name)
      return(*it);
  }

  return NULL;
}

PlugBox* GearGui::getOutputPlugBox(std::string name) const
{
  for (std::vector<PlugBox*>::const_iterator it = _outputPlugBoxes.begin();it!=_outputPlugBoxes.end();++it)
  {
    if ((*it)->plug()->name() == name)
      return(*it);
  }

  return NULL;
}

int GearGui::rtti() const
{
  return CANVAS_RTTI_GEAR;
}

void GearGui::timerEvent(QTimerEvent* timerEvent)
{
  //cannot update if not on a canvas
  if (canvas()==NULL)
    return;

  reDraw();
}

void GearGui::reDraw()
{
  if (canvas()!=NULL)
  {
    update();    
    canvas()->update();    
  }
}
