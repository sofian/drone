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
#include <qcanvas.h>
#include <qobject.h>


class Gear;
class PlugBox;
class Engine;
class QDomElement;
class QDomDocument;

class GearGui : public QObject, public QCanvasRectangle
{
Q_OBJECT	
public:
  static const int CANVAS_RTTI_GEAR;
  static const int DEFAULT_SIZEX;
	static const int DEFAULT_SMALL_SIZEX;
  static const int NAME_SIZEY;
  static const int PLUGBOXES_NOMINAL_INTERVAL;
  static const int PLUGBOXES_STARTING_OFFSET;
  static const int SHADOW_OFFSET;
	static const int RENDERING_OFFSET;

  //colors    
  static const QColor BOX_COLOR;  
  static const QColor SELECTED_BOX_COLOR;  
  static const QColor CUTTED_BOX_COLOR;  
  static const QColor BOXNAME_COLOR;
  static const QColor SHADOW_COLOR;
	static const QColor GEAR_READY_COLOR;
	static const QColor GEAR_NOT_READY_COLOR;

	
  //fonts
  static const QFont NAME_FONT;
	
  virtual int rtti() const;

  GearGui(Gear *pgear, QCanvas *canvas, QColor color=BOXNAME_COLOR, int sizeX=DEFAULT_SIZEX, 
					int sizeY=0, int smallSizeX=DEFAULT_SMALL_SIZEX, int smallSizeY=0, int updateRate=-1);
  virtual ~GearGui();

  int renderingStartX();
  int renderingStartY();	
  
  void refresh();
	
  PlugBox* getInputPlugBox(std::string name) const;
  PlugBox* getOutputPlugBox(std::string name) const;

  Gear* gear(){return _gear;};

  bool titleBarHitted(const QPoint& p);

  PlugBox* plugHitted(const QPoint& p);
  void performPlugHighligthing(const QPoint &p);
  void performPlugHighligthing(PlugBox *plugBox);
  void unHilightAllPlugBoxes();
  
  virtual void save(QDomDocument &doc, QDomElement &gearElem);
  virtual void load(QDomElement &gearElem);

  //! returns true if the function eats up the event
  virtual bool mouseEvent(const QPoint&, Qt::ButtonState){return false;}
  virtual bool keyEvent(QKeyEvent *){return false;}

  void getDrawableArea(int *ox, int *oy, int *sizeX, int *sizeY);
  void setTitle(std::string title){_title=title;}
 
  void setSelected(bool state){_selected=state;QCanvasRectangle::setSelected(state);}
  void toggleSelection(){setSelected(!_selected);}
  bool isSelected() const {return _selected;}
	
	void showSmall(bool v);

public slots:
	void reDraw();
	
protected:
				
  void timerEvent(QTimerEvent*);

  void drawShape(QPainter &painter);
  virtual void drawNormal(QPainter &painter);
  virtual void drawSmall(QPainter &painter);
	
  void drawPlugBoxes(QPainter &painter);

  void determineMinimumSize();

  Gear *_gear;
  std::vector<PlugBox*> _inputPlugBoxes;
  std::vector<PlugBox*> _outputPlugBoxes;
  std::vector<PlugBox*> _plugBoxes;//!contain all inputs and outputs

  bool _selected;

  // the effective Size (can be stretched by user)
  int _sizeX;
  int _sizeY;
	int _normalSizeX;
	int _normalSizeY;
	int _smallSizeX;
	int _smallSizeY;
	
  // minimum required size for gear
  int _minSizeX;
  int _minSizeY;

  int _inputsInterval;
  int _outputsInterval;
  QColor _boxNameColor;
  std::string _title;
private:
  void removeAllPlugBoxes();
  pthread_mutex_t *_reDrawMutex;
	bool _showSmall;//! draw a small version of the gear

};

#endif

