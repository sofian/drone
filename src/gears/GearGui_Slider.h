#ifndef GEARGUI_SLIDER_INCLUDED
#define GEARGUI_SLIDER_INCLUDED

#include "GearGui.h"


#include <qcanvas.h>
#include <vector>

class Gear_Slider;

class GearGui_Slider : public GearGui
{
public:
  static const int SLIDER_THICKNESS;    
  static const int SLIDER_SIZEY;
  static const int SLIDER_SIZEX;
  static const int SLIDER_LINE_OFFSETY;
  static const QColor SLIDER_COLOR;
  static const QColor SLIDER_BOX_COLOR;

  GearGui_Slider(Gear_Slider *gear, QCanvas *canvas);

  bool mouseEvent(const QPoint& p, Qt::ButtonState button);
  void moveSlider(int sliderPos);

protected:


  void drawShape(QPainter &painter);


};


#endif

