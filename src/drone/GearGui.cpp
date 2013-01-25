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
#include "GearControl.h"
#include "Engine.h"
#include "PlugBox.h"
#include "Control.h"
#include "ThreadUtil.h"
#include <iostream>
#include <sstream>
#include "SchemaGui.h"
#include "XMLHelper.h"
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

// space between plugs and center (control) region 
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


 
GearGui::GearGui(Gear *pgear, QColor color) :
QObject(),
QGraphicsRectItem(),
_gear(pgear),
_layoutMode(normal),
_size(100,100),
_controlWidth(0),
_controlHeight(0),
_control(NULL),
_longestInputPlugName(0),
_longestOutputPlugName(0),
_boxNameColor(color)
{
  
  setFlags(ItemIsSelectable | ItemIsMovable );
  //not needed for now
  //setAcceptsHoverEvents(true);
  
//  setCacheMode(ItemCoordinateCache);
  // specify an update rate to periodically refresh gear control (ex : GearTV)  
  //if (updateRate>=0)
  //  startTimer(updateRate);


  // setup drop shadow effect
  QGraphicsDropShadowEffect* dropShadow = new QGraphicsDropShadowEffect();
  dropShadow->setBlurRadius(20);
  dropShadow->setOffset(SHADOW_OFFSET);
  dropShadow->setColor(QColor(0, 0, 0, 120));
  setGraphicsEffect(dropShadow);
  QObject::connect(_gear, SIGNAL(readyStatusChanged()), this, SLOT(redraw()));

  bool rebuildLayoutDone=false;
  GearControl *gear_c;
  // Is this a GearControl instance ? 
  if((gear_c=qobject_cast<GearControl*>(pgear)))
  {
    // If so, then check if it has a controller.
    // If it has none, it's likely because it's instanciated from the core, without a GUI
    if(gear_c->getControl()!=NULL)
    {
      setControl(gear_c->getControl());
      rebuildLayoutDone=true;
    }
  }
  if(!rebuildLayoutDone)
      rebuildLayout();

  
  
  
}

GearGui::~GearGui()
{
  GearControl* controlGear;

  //delete all the plugBoxes
  //plugboxes take care of deleting connectionItems when deleted
  //Everything is taken care of ! :)
  removeAllPlugBoxes();

  if ((controlGear = qobject_cast<GearControl*>(_gear)))
  {
    if (controlGear->getControl())
      delete controlGear->getControl();
  }

}


QRectF GearGui::boundingRect() const
{
    return QRectF(-6, -2, _size.width()+12, _size.height()+4);
}

QPainterPath GearGui::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, _size.width(), _size.height());
    return path;
}

void GearGui::setControl(Control* control)
{
  _control=control;
  _control->setParentItem(this);
  _layoutMode = noPlugNames;
  QObject::connect(control,SIGNAL(geometryChanged()),this,SLOT(rebuildLayout()));
  rebuildLayout();
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
  
  double ratio = (y-titleBarHeight()) / (_size.height() - titleBarHeight());
  return GearGui::interpolateColors(ratio,c1,c2);

}

void GearGui::redraw()
{
  update();
}


void GearGui::setLayoutMode(layoutMode mode)
{
  _layoutMode=mode;
  rebuildLayout();
}

//! recompute layout and recreate plugBoxes from the parent gear plugs
void GearGui::rebuildLayout()
{
  // compute gear name width
  QFontMetrics fm(NAME_FONT);
  qreal name_width = fm.width(_gear->name());
  qreal sizeX,sizeY;
  
  if(_control!=NULL)
  {
    _controlWidth = _control->size().width();
    _controlHeight = _control->size().height();
  }
  else
    _controlWidth = _controlHeight = 0;
  
  //delete and remove inputplugboxes that we dont have anymore
  //we also erase plugs in inputs that we already have
  QList<AbstractPlug*> inputs;
  _gear->getInputs(inputs);
  
  //delete and remove output plugboxes that we dont have anymore
  //we also erase plugs in outputs that we already have
  QList<AbstractPlug*> outputs;
  _gear->getOutputs(outputs);
  
  
  removeAllPlugBoxes();

  
  
  int maxPlugs = inputs.size() > outputs.size() ? inputs.size() : outputs.size();

  // compute minimal height
  sizeY = (maxPlugs * PLUGBOX_VERTICAL_SPACING) + MARGIN_TOP + MARGIN_BOTTOM;
  sizeY = qMax(sizeY, _controlHeight);
  sizeY += titleBarHeight();

  sizeX=0;

  // add plug name width if they are visible
  _longestInputPlugName = 0;
  _longestOutputPlugName = 0;
  if (_layoutMode == normal)
  {
    foreach(AbstractPlug* in,inputs)
      _longestInputPlugName = qMax(_longestInputPlugName, PlugBox::plugNameWidth(in->name()));
    sizeX += _longestInputPlugName;

    foreach(AbstractPlug* out,outputs)
      _longestOutputPlugName = qMax(_longestOutputPlugName, PlugBox::plugNameWidth(out->name()));
    sizeX += _longestOutputPlugName;
  
  }
  else 
  {
    if(inputs.count())
      sizeX+=PlugBox::PLUGBOX_RADIUS + 3;
    if(outputs.count())
      sizeX+=PlugBox::PLUGBOX_RADIUS + 3;
  }
  sizeX+=_controlWidth;
  sizeX=qMax(sizeX,name_width + 2*MARGIN_NAME);
  sizeX+=MARGIN_SIDE*2;
  
  
  qreal by=0;
  qreal bx=0;
  
  //now create plugboxes
  PlugBox *plugBox;
  by = MARGIN_TOP + titleBarHeight() +  PLUGBOX_VERTICAL_OFFSET;
    
  foreach(AbstractPlug* in,inputs)
  {
    
    plugBox = new PlugBox(in, this);
    plugBox->setPos(QPointF(0, by));
    _plugBoxes<<plugBox;
    _inputPlugBoxes<<plugBox;
    by+=PLUGBOX_VERTICAL_SPACING;
  }
  
  by = MARGIN_TOP + titleBarHeight() + PLUGBOX_VERTICAL_OFFSET;
  bx = sizeX - PlugBox::PLUGBOX_RADIUS;
    
  foreach(AbstractPlug* out,outputs)
  {
    plugBox = new PlugBox(out, this);
    plugBox->setPos(QPointF(bx, by));
    _plugBoxes<<plugBox;
    _outputPlugBoxes<<plugBox;
    by+=PLUGBOX_VERTICAL_SPACING;
  }
  
  _size = QSizeF(sizeX,sizeY);
  if(_control)
  {
        qDebug()<<"##"<<_size.width() <<";"<< MARGIN_SIDE*2 <<";"<< _longestInputPlugName <<";"<< _longestOutputPlugName <<";"<< _control->size().width();
        qDebug()<<"@@"<<(name_width + 2*MARGIN_NAME);
    qreal extraControlWidth = _size.width() - (MARGIN_SIDE*2 + _longestInputPlugName + _longestOutputPlugName + _control->size().width());
    qreal extraControlHeight = _size.height() - (titleBarHeight() + _control->size().height());
        qDebug()<<"!!"<<extraControlWidth<<":"<<(MARGIN_SIDE + _longestInputPlugName + extraControlWidth/2);
    _control->setPos(MARGIN_SIDE + _longestInputPlugName + extraControlWidth/2, titleBarHeight() + extraControlHeight/2);
    
  }
  update();
}

void GearGui::removeAllPlugBoxes()
{
  //delete all the plugBoxes
  //plugboxes take care of deleting connectionItems when deleted
  //Everything is taken care of ! :)
  qDeleteAll(_plugBoxes);
  _plugBoxes.clear();
  _inputPlugBoxes.clear();
  _outputPlugBoxes.clear();
}

QColor GearGui::getCurrentColor()
{
  return isSelected()?BOX_COLOR.light(125):BOX_COLOR;
}


void GearGui::paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    
   bool selected = (option->state & QStyle::State_Selected);
   //qDebug()<<_gear->getUUID()<<":"<<selected;
   //bool hover = (option->state & QStyle::State_MouseOver);

//   setOpacity(gear()->ready()?0.97:0.6); 
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
    QLinearGradient gradient2(0, 0, 0, _size.height());
    gradient2.setColorAt(0, SELECT_OUTLINE_TOP);
    gradient2.setColorAt(1, SELECT_OUTLINE_BOTTOM);
    painter->setBrush(gradient2);
    painter->setPen(Qt::NoPen);
    painter->drawRoundRect(QRectF(-2, -2, _size.width()+4, _size.height()+4), this->radiusHelper(_size.width()+4, 16), this->radiusHelper(_size.height()+4, 16));
    foreach(PlugBox* ipb,_inputPlugBoxes)
      ipb->drawSelected(painter);
    foreach(PlugBox* opb,_outputPlugBoxes)
      opb->drawSelected(painter);
  
  }

  painter->setPen(pen);
  
  
  QLinearGradient gradient(0, 0, 0, _size.height());
  gradient.setColorAt(0, fillColor.lighter(300));
  gradient.setColorAt(pixToGradientPos(2,_size.height()), fillColor.lighter(150));
  gradient.setColorAt(pixToGradientPos(TITLE_BAR_HEIGHT,_size.height()), fillColor.lighter(130)); 
  gradient.setColorAt(pixToGradientPos(TITLE_BAR_HEIGHT+1,_size.height()), fillColor); 
  gradient.setColorAt(1, fillColor.darker(BOX_GRADIENT_DARKER_AMOUT)); 
  painter->setBrush(gradient);
  
  painter->drawRoundRect(QRectF(0,0,_size.width(),_size.height()),this->radiusHelper(_size.width(),14),this->radiusHelper(_size.height(),14));


  //gear name
  painter->setBrush(_boxNameColor.lighter(selected?200:100));

  painter->setFont(NAME_FONT);
  painter->setPen(Qt::white);

  //title default to gear name if not explicitly set
  QString title=_title;
  if (title.isEmpty())
    title=_gear->instanceName();
	
  painter->drawText(0,1, _size.width(), TITLE_BAR_HEIGHT, Qt::AlignHCenter | Qt::AlignVCenter, title);
  
	//ready box
  //painter->setPen(Qt::black);
  //painter->setBrush(gear()->ready() ? GEAR_READY_COLOR : GEAR_NOT_READY_COLOR);
  //painter->drawRoundRect(sizeX - 10, 2, 8, 8, 50, 50);

  //plugboxes
  foreach(PlugBox* ipb,_inputPlugBoxes)
    ipb->draw(painter);
  foreach(PlugBox* opb,_outputPlugBoxes)
    opb->draw(painter);

}

// test for plug collision in gear coordinates
PlugBox* GearGui::plugHit(const QPointF &p)
{
  foreach(PlugBox* pb,_plugBoxes)
  {
    if (pb->hit(p))
      return(pb);
  }

  return NULL;
}

void GearGui::performPlugHighligthing(const QPointF &p)
{
  //PlugBox *plugbox;

  
  foreach(PlugBox* plugbox,_plugBoxes)
  {
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
  foreach(PlugBox* pb,_plugBoxes)
    if (pb!=plugBox)    
      pb->hilight(false);
  
  if (plugBox)
    plugBox->hilight(true);
  
  update();
}

void GearGui::unHilightAllPlugBoxes()
{
  foreach(PlugBox* pb,_plugBoxes)
  pb->hilight(false);

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

  QDomAttr sel = doc.createAttribute("Sel");
  sel.setValue(isSelected()?"1":"0");
  gearElem.setAttributeNode(sel);

  QDomAttr z = doc.createAttribute("Z");
  QString tmpStr(QString("%1").arg((int)zValue()));
  z.setValue(tmpStr);
  gearElem.setAttributeNode(z);
}                                                                                                                       

void GearGui::load(QDomElement &gearElem)                        
{
  setX(gearElem.attribute("PosX","").toFloat());
  setY(gearElem.attribute("PosY","").toFloat());
  setSelected(gearElem.attribute("Sel","").toInt());
  setZValue(gearElem.attribute("Z","").toInt());
  
  //load plugs attributes
  QDomNode gearGuiNode = XMLHelper::findChildNode(gearElem, "GearGui");

  if(!gearGuiNode.isNull())
  {
    QDomElement gearGuiElem = gearGuiNode.toElement();
    if (!gearGuiElem.isNull())
    {
      setX(gearGuiElem.attribute("PosX","").toInt());
      setY(gearGuiElem.attribute("PosY","").toInt());
		}
	}
}

PlugBox* GearGui::getPlugBox(QString name) const
{

  foreach(PlugBox* pb, _plugBoxes)
  {
    if (pb->plug()->name() == name)
      return (pb);
  }

  return NULL;
}


PlugBox* GearGui::getInputPlugBox(QString name) const
{
    foreach(PlugBox* ipb,_inputPlugBoxes)
{
    if (ipb->plug()->name() == name)
      return(ipb);
  }

  return NULL;
}

PlugBox* GearGui::getOutputPlugBox(QString name) const
{
  foreach(PlugBox* opb,_outputPlugBoxes)
  {
    if (opb->plug()->name() == name)
      return(opb);
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

