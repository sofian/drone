/* GearGui.h
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

#ifndef GEARGUI_INCLUDED
#define GEARGUI_INCLUDED

#include <vector>
#include <QGraphicsRectItem>
#include <qobject.h>
//Added by qt3to4:
#include <QTimerEvent>
#include <QKeyEvent>
#include <QLinearGradient>
#include <QColor>

class Control;
class Gear;
class PlugBox;
class Engine;
class QDomElement;
class QDomDocument;

class GearGui : public QObject, public QGraphicsRectItem
{
Q_OBJECT	
public:

  enum { Type = UserType + 1 };

  // GearuGui Layout Modes : 
  // - normal (drone standard)
  // - noPlugNames (normal but without plug names)
  // - compact (no more title bar nor plug names, title in the middle)
  // - ultracompact (like compact without title in the middle)
  enum layoutMode {
    normal, noPlugNames, compact, ultracompact
  };


  static const int CANVAS_RTTI_GEAR;
  static const qreal DEFAULT_SIZEX;
  static const qreal TITLE_BAR_HEIGHT;
  static const qreal PLUGBOX_VERTICAL_SPACING;
  static const qreal PLUGBOX_VERTICAL_OFFSET;
  static const qreal MARGIN_NAME;
  static const qreal MARGIN_SIDE;
  static const qreal MARGIN_TOP;
  static const qreal MARGIN_BOTTOM;
  static const qreal SHADOW_OFFSET;
  static const qreal RENDERING_OFFSET;

  //colors    
  static const QColor BOX_COLOR;  
  static const int BOX_GRADIENT_DARKER_AMOUT;  
  static const QColor SELECT_OUTLINE_TOP;  
  static const QColor SELECT_OUTLINE_BOTTOM;  
  static const QColor BOXNAME_COLOR;
  static const QColor GEAR_READY_COLOR;
  static const QColor GEAR_NOT_READY_COLOR;

	
  //fonts
  static const QFont NAME_FONT;
	
  virtual int rtti() const;

  GearGui(Gear *pgear, QColor color=BOXNAME_COLOR);
  virtual ~GearGui();

  int renderingStartX();
  int renderingStartY();	
  
  
  QRectF boundingRect() const;
  QPainterPath shape() const;

  PlugBox* getInputPlugBox(QString name) const;
  PlugBox* getOutputPlugBox(QString name) const;

  Gear* gear(){return _gear;};

  QColor colorAtY(qreal y);
  QColor getCurrentColor();

  PlugBox* plugHit(const QPointF& p);
  void performPlugHighligthing(const QPointF &p);
  void performPlugHighligthing(PlugBox *plugBox);
  void unHilightAllPlugBoxes();
  
  virtual void save(QDomDocument &doc, QDomElement &gearElem);
  virtual void load(QDomElement &gearElem);

  //! returns true if the function eats up the event
  virtual bool mouseEvent(const QPoint&, Qt::ButtonState){return false;}
  virtual bool keyEvent(QKeyEvent *){return false;}

  void getDrawableArea(int *ox, int *oy, int *sizeX, int *sizeY);
  void setTitle(QString title){_title=title;}

  void setLayoutMode(layoutMode mode);  
  layoutMode getLayoutMode() const {return _layoutMode;}
  qreal titleBarHeight(){return (_layoutMode==normal || _layoutMode==noPlugNames)?TITLE_BAR_HEIGHT:0;}  
  QSizeF size() const {return _size;}

  void setControl(Control* control);
  
  int type() const {return Type;}

public slots: 
  void redraw();
  void rebuildLayout();
	
protected:
  
  void timerEvent(QTimerEvent*);

  virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
  
  Gear *_gear;
  QList<PlugBox*> _inputPlugBoxes;
  QList<PlugBox*> _outputPlugBoxes;
  QList<PlugBox*> _plugBoxes;//!contain all inputs and outputs

  bool _selected;
  layoutMode _layoutMode;
  
  // the effective Size (can be stretched by user)
  QSizeF _size;

  // minimum required size for controller region (space at center of a gear where something custom can be drawn)
  qreal _controlWidth;
  qreal _controlHeight;
  
  // The control that is at the center 
  Control* _control;
  
  qreal _longestInputPlugName, _longestOutputPlugName;
  QColor _boxNameColor;
  QString _title;
private:
  QColor interpolateColors(qreal ratio, const QColor &c1, const QColor &c2) const;
  inline qreal radiusHelper(qreal pix, qreal goalRadius){return 100.0*goalRadius/pix;}
  inline qreal pixToGradientPos(qreal pix, qreal gradLength){return pix/gradLength;}
  void removeAllPlugBoxes();

};

#endif

