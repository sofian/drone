#ifndef GEARGUI_TV_INCLUDED
#define GEARGUI_TV_INCLUDED

#include <qcanvas.h>
#include <vector>
#include <qimage.h>


#include "GearGui.h"
#include "Gear_TV.h"


class GearGui_TV : public GearGui
{
public:

  GearGui_TV(Gear_TV *gear, QCanvas *canvas);


protected:
  QImage _videoFrame;    

  void drawShape(QPainter &painter);




private:

  static const QColor NO_SIGNAL_COLOR;
  static const int TV_SIZEX;
  static const int TV_SIZEY;
  static const int UPDATE_RATE_MS;

  int _currentSizeX;
  int _currentSizeY;



};

#endif

