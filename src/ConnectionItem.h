#ifndef CONNECTIONITEM_INCLUDED
#define CONNECTIONITEM_INCLUDED


#include <qcanvas.h>
class PlugBox;
class Engine;

class QDomDocument;
class QDomElement;

class ConnectionItem : public QCanvasLine
{
public:
  static const int CANVAS_RTTI_CONNECTION;
  enum eState
  {
    DISCONNECTED, CONNECTING, CONNECTED
  };

  ConnectionItem(Engine *engine, QCanvas *canvas);
  virtual ~ConnectionItem();

  int rtti () const {return CANVAS_RTTI_CONNECTION;};

  void setStartingPlugBox(PlugBox *plugBox);
  void setConnectionLineEndPoint(QPoint const &p);

  PlugBox *sourcePlugBox();
  PlugBox *destPlugBox();

  bool createConnection(PlugBox *plugBox);

  void createConnectionLineOnly(PlugBox *source, PlugBox *dest);

  void hiLight(bool hi);

protected:

//    void updateAreaPoints();
  void drawShape(QPainter &painter);

  void getOrigin(int *x, int *y);
  void getDest(int *x, int *y);

  eState _state;

  bool _hiLighted;
  int _destPointX,_destPointY;

  PlugBox* _sourcePlugBox;
  PlugBox* _destPlugBox;

  Engine *_engine;

  QPen* _pen;

};


#endif
