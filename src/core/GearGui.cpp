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
#include "ThreadUtil.h"
#include <iostream>
#include <sstream>
#include "SchemaGui.h"
#include <qpainter.h>
#include <qdom.h>
//Added by qt3to4:
#include <QTimerEvent>
#include <QGraphicsScene>
#include <QGraphicsDropShadowEffect>
#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>
#include <QGraphicsSceneMouseEvent>

const int GearGui::CANVAS_RTTI_GEAR = 2000;


// default title bar height
const qreal GearGui::TITLE_BAR_HEIGHT = 14;
// vertical space in pixels between two plugs

// space between plugs and center (controller) region 
const qreal GearGui::MARGIN_SIDE = 0;
// space to the left and right of gear name 
const qreal GearGui::MARGIN_NAME = 5;
const qreal GearGui::MARGIN_TOP = 5;
const qreal GearGui::MARGIN_BOTTOM = 5;


const qreal GearGui::PLUGBOX_VERTICAL_SPACING = 16;
// vertical offset from title bar to first plug
const qreal GearGui::PLUGBOX_VERTICAL_OFFSET = 4;
// shadow offset in pixels 
const qreal GearGui::SHADOW_OFFSET = 4;
const qreal GearGui::RENDERING_OFFSET = 2;

// default gear color
const QColor GearGui::BOX_COLOR(60,95,50);
const int GearGui::BOX_GRADIENT_DARKER_AMOUT(125);
const QColor GearGui::BOXNAME_COLOR(0,31,68,200);

const QColor GearGui::GEAR_READY_COLOR(36, 179, 42);
const QColor GearGui::GEAR_NOT_READY_COLOR(78, 88, 99);

const QColor GearGui::SELECT_OUTLINE_TOP(255, 225, 0);
const QColor GearGui::SELECT_OUTLINE_BOTTOM(225, 155, 0);


const QFont GearGui::NAME_FONT("Roboto", 10, QFont::Normal);


QRectF GearGui::boundingRect() const
{
    return QRectF(-6, -2, _sizeX+12, _sizeY+4);
}

QPainterPath GearGui::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, _sizeX, _sizeY);
    return path;
}

// this should probably be moved in a helper class
// interpolates between 2 color in RGB space
// ratio=0 will return c1, ratio=1 will return c2
QColor GearGui::interpolateColors(qreal ratio, const QColor &c1, const QColor &c2) const
{
  int c1r, c1g, c1b;
  int c2r, c2g, c2b;
  c1.getRgb(&c1r, &c1g, &c1b);
  c2.getRgb(&c2r, &c2g, &c2b);
  int red = (int) (ratio * c2r + (1 - ratio) * c1r); //in your case, the values are 12 and 122
  int green = (int) (ratio * c2g + (1 - ratio) * c1g); //in your case, the values are 23 and 233
  int blue = (int) (ratio * c2b + (1 - ratio) * c1b); //in your case, the values are 24 and 244
  return QColor(red, green, blue);

}


// returns the color of the gear at position Y (in gear coordinate system). 
// Currently used by PlugBox during their painting function
QColor GearGui::colorAtY(qreal y) 
{
  QColor c1(getCurrentColor()), c2;
  c2 = c1.darker(BOX_GRADIENT_DARKER_AMOUT);
  
  double ratio = (y-titleBarHeight()) / (_sizeY - titleBarHeight());
  return GearGui::interpolateColors(ratio,c1,c2);

}
 
GearGui::GearGui(Gear *pgear, QGraphicsScene *scene, QColor color, qreal controllerWidth, qreal controllerHeight, int updateRate) :
QObject(),
QGraphicsRectItem(),
_gear(pgear),
_selected(false),
_layoutMode(normal),
_sizeX(100),
_sizeY(100),
_controllerWidth(controllerWidth),
_controllerHeight(controllerHeight),
_boxNameColor(color)
{
  
  setFlags(ItemIsSelectable | ItemIsMovable );
  //not needed for now
  //setAcceptsHoverEvents(true);
  
  
  // specify an update rate to periodically refresh gear control (ex : GearTV)  
  if (updateRate>=0)
    startTimer(updateRate);

  rebuildLayout();

  // setup drop shadow effect
  QGraphicsDropShadowEffect* dropShadow = new QGraphicsDropShadowEffect();
  dropShadow->setBlurRadius(20);
  dropShadow->setOffset(SHADOW_OFFSET);
  dropShadow->setColor(QColor(0, 0, 0, 120));
  setGraphicsEffect(dropShadow);
  setOpacity(0.97);
  QObject::connect(_gear, SIGNAL(readyStatusChanged()), this, SLOT(redraw()));
}

void GearGui::redraw()
{
  update();
}

GearGui::~GearGui()
{
  //delete all the plugBoxes
  //plugboxes take care of deleting connectionItems when deleted
  //Everything is taken care of ! :)
  removeAllPlugBoxes();
}

void GearGui::setLayoutMode(layoutMode mode)
{
  _layoutMode=mode;
  rebuildLayout();
}

int GearGui::renderingStartX()
{
	return RENDERING_OFFSET;
}

int GearGui::renderingStartY()
{
	return RENDERING_OFFSET;
}

//! recompute layout and recreate plugBoxes from the parent gear plugs
void GearGui::rebuildLayout()
{
  // compute gear name width
  QFontMetrics fm(NAME_FONT);
  qreal name_width = fm.width(_gear->name().c_str());

  //delete and remove inputplugboxes that we dont have anymore
  //we also erase plugs in inputs that we already have
  std::list<AbstractPlug*> inputs;
  _gear->getInputs(inputs);
  
  //delete and remove output plugboxes that we dont have anymore
  //we also erase plugs in outputs that we already have
  std::list<AbstractPlug*> outputs;
  _gear->getOutputs(outputs);
  
  
  removeAllPlugBoxes();

  
  
  int maxPlugs = inputs.size() > outputs.size() ? inputs.size() : outputs.size();

  // compute minimal height
  _sizeY = (maxPlugs * PLUGBOX_VERTICAL_SPACING) + MARGIN_TOP + MARGIN_BOTTOM;
  _sizeY = qMax(_sizeY, _controllerHeight);
  _sizeY += titleBarHeight();

  _sizeX=0;

  // add plug name width if they are visible
  if (_layoutMode == normal)
  {
    qreal longestInputPlugName = 0;
    for (std::list<AbstractPlug*>::iterator it = inputs.begin(); it != inputs.end(); ++it)
      longestInputPlugName = qMax(longestInputPlugName, PlugBox::plugNameWidth((*it)->name()));
    _sizeX += longestInputPlugName;

    qreal longestOutputPlugName = 0;
    for (std::list<AbstractPlug*>::iterator it = outputs.begin(); it != outputs.end(); ++it)
      longestOutputPlugName = qMax(longestOutputPlugName, PlugBox::plugNameWidth((*it)->name()));
    _sizeX += longestOutputPlugName;
  
  }
  else _sizeX+=PlugBox::PLUGBOX_RADIUS * 2 + 6;
  _sizeX+=_controllerWidth;
  _sizeX=qMax(_sizeX,name_width + 2*MARGIN_NAME);
  _sizeX+=MARGIN_SIDE*2;
  
  
  qreal by=0;
  qreal bx=0;
  
  //now create plugboxes
  PlugBox *plugBox;
  by = MARGIN_TOP + titleBarHeight() +  PLUGBOX_VERTICAL_OFFSET;
    
  for (std::list<AbstractPlug*>::iterator it = inputs.begin(); it != inputs.end(); ++it)
  {
    
    plugBox = new PlugBox(*it, this);
    plugBox->setPos(QPointF(0, by));
    _plugBoxes.push_back(plugBox);
    _inputPlugBoxes.push_back(plugBox);
    by+=PLUGBOX_VERTICAL_SPACING;
  }
  
  by = MARGIN_TOP + titleBarHeight() + PLUGBOX_VERTICAL_OFFSET;
  bx = _sizeX - PlugBox::PLUGBOX_RADIUS;
    
  for (std::list<AbstractPlug*>::iterator it = outputs.begin(); it != outputs.end(); ++it)
  {
    plugBox = new PlugBox(*it, this);
    plugBox->setPos(QPointF(bx, by));
    _plugBoxes.push_back(plugBox);
    _outputPlugBoxes.push_back(plugBox);
    by+=PLUGBOX_VERTICAL_SPACING;
  }
  
  update();
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

  *ox = (int)renderingStartX() + PlugBox::PLUGBOX_RADIUS + 4;
  *oy = (int)renderingStartY() + GearGui::TITLE_BAR_HEIGHT + 4;
  *sizeX = _sizeX - (PlugBox::PLUGBOX_RADIUS+4)*2;
  *sizeY = _sizeY - GearGui::TITLE_BAR_HEIGHT - 10;            
}

QColor GearGui::getCurrentColor()
{
  return isSelected()?BOX_COLOR.light(125):BOX_COLOR;
}


void GearGui::paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    
   bool selected = (option->state & QStyle::State_Selected);
   bool hover = (option->state & QStyle::State_MouseOver);

   QColor fillColor(getCurrentColor());

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if (lod < 0.2) {
    /// do simple painting and return
    }

    // hack to compensate the fact that in Qt, 1 pixel wide stroke on a round 
    // integer coordinate sits exactly in the middle of 2 pixels...  
    painter->translate(.5 / painter->transform().m11(), .5 / painter->transform().m22());
    
  QPen pen;
  pen.setWidthF(1);
  if(selected)
{
    QLinearGradient gradient2(0, 0, 0, _sizeY);
    gradient2.setColorAt(0, SELECT_OUTLINE_TOP);
    gradient2.setColorAt(1, SELECT_OUTLINE_BOTTOM);
    painter->setBrush(gradient2);
    painter->setPen(Qt::NoPen);
    painter->drawRoundRect(QRectF(-2, -2, _sizeX+4, _sizeY+4), this->radiusHelper(_sizeX+4, 16), this->radiusHelper(_sizeY+4, 16));
    for (std::vector<PlugBox*>::iterator it = _inputPlugBoxes.begin(); it != _inputPlugBoxes.end(); ++it)
      (*it)->drawSelected(painter);
    for (std::vector<PlugBox*>::iterator it = _outputPlugBoxes.begin(); it != _outputPlugBoxes.end(); ++it)
      (*it)->drawSelected(painter);
  
  }

  painter->setPen(pen);
  
  
  QLinearGradient gradient(0, 0, 0, _sizeY);
  gradient.setColorAt(0, fillColor.lighter(300));
  gradient.setColorAt(pixToGradientPos(2,_sizeY), fillColor.lighter(150));
  gradient.setColorAt(pixToGradientPos(TITLE_BAR_HEIGHT,_sizeY), fillColor.lighter(130)); 
  gradient.setColorAt(pixToGradientPos(TITLE_BAR_HEIGHT+1,_sizeY), fillColor); 
  gradient.setColorAt(1, fillColor.darker(BOX_GRADIENT_DARKER_AMOUT)); 
  painter->setBrush(gradient);
  
  painter->drawRoundRect(QRectF(0,0,_sizeX,_sizeY),this->radiusHelper(_sizeX,14),this->radiusHelper(_sizeY,14));


  //gear name
  painter->setBrush(_boxNameColor.lighter(selected?200:100));

  painter->setFont(NAME_FONT);
  painter->setPen(Qt::white);

  //title default to gear name if not explicitly set
  std::string title=_title;
  if (title.empty())
    title=_gear->name();
	
  painter->drawText(0,1, _sizeX, TITLE_BAR_HEIGHT, Qt::AlignHCenter | Qt::AlignVCenter, title.c_str());
  
	//ready box
  painter->setPen(Qt::black);
  painter->setBrush(gear()->ready() ? GEAR_READY_COLOR : GEAR_NOT_READY_COLOR);
  painter->drawRoundRect(_sizeX - 10, 2, 8, 8, 50, 50);

  //plugboxes
  for (std::vector<PlugBox*>::iterator it = _inputPlugBoxes.begin(); it != _inputPlugBoxes.end(); ++it)
    (*it)->draw(painter);
  for (std::vector<PlugBox*>::iterator it = _outputPlugBoxes.begin(); it != _outputPlugBoxes.end(); ++it)
    (*it)->draw(painter);

}

// test for plug collision in gear coordinates
PlugBox* GearGui::plugHit(const QPointF &p)
{
  for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)
  {
    if ((*it)->hit(p))
      return(*it);
  }

  return NULL;
}

void GearGui::performPlugHighligthing(const QPointF &p)
{
  PlugBox *plugbox;

  
  for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)
  {
    plugbox = *it;

    if (plugbox->hit(p))
      plugbox->hilight(true);
    else
      plugbox->hilight(false);
  }

  //we need to repaint ourself
  update();
}

void GearGui::performPlugHighligthing(PlugBox *plugBox)
{
  for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)  
    if ((*it)!=plugBox)    
      (*it)->hilight(false);
  
  if (plugBox)
    plugBox->hilight(true);
  
update();
}

void GearGui::unHilightAllPlugBoxes()
{
  for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)
    (*it)->hilight(false);

  update();
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

void GearGui::timerEvent(QTimerEvent*)
{
  //cannot update if not on a scene
  update();
}

