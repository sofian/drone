#ifndef GEARGUI_PUSHBUTTON_INCLUDED
#define GEARGUI_PUSHBUTTON_INCLUDED

#include "GearGui.h"


#include <qcanvas.h>
#include <vector>

class Gear_PushButton;

class GearGui_PushButton : public GearGui
{
public:
  static const int PUSHBUTTON_SIZEY;
  static const int PUSHBUTTON_SIZEX;
  static const QColor PUSHBUTTON_COLOR;
  static const QColor PUSHBUTTON_BOX_COLOR;
  static const QColor PUSHBUTTON_BOX_COLORON;

  GearGui_PushButton(Gear_PushButton *gear, QCanvas *canvas);

  bool mouseEvent(const QPoint& p, Qt::ButtonState button);
  bool keyEvent(QKeyEvent *e);
  
protected:

  void drawShape(QPainter &painter);


};


#endif

