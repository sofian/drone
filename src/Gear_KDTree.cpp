#include "Gear_KDTree.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_KDTree, Gear_KDTree, "KDTree")

  Gear_KDTree::Gear_KDTree(Engine *engine, std::string name)
    : Gear(engine, "KDTree", name)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  //  _AMOUNT_IN = addPlugSignalIn("Depth", DEFAULT_N_COLORS);
  _rasterer = new Rasterer();
  _table = new SummedAreaTable();
}

Gear_KDTree::~Gear_KDTree()
{
  delete _rasterer;
  delete _table;
}

void Gear_KDTree::init()
{
  _rasterer->setCanvas(_VIDEO_OUT->canvas());
  _rasterer->setColor(0, 0, 0); // black lines only
  _table->setCanvas(_VIDEO_IN->canvas());
}


bool Gear_KDTree::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_KDTree::runVideo()
{
  // initialize
  
  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());
  //_image->allocate(_image->sizeX(), _image->sizeY());

  _sizeX = _image->sizeX();
  _sizeY = _image->sizeY();
  _size = _sizeX * _sizeY;

  _data = _image->_data;
  _outData = _outImage->_data;

  _rasterer->setCanvas(_outImage);
  _table->setCanvas(_image);

  // build accumulation buffer
  _table->buildTable();
  
  memcpy(_outData, _data, _size * sizeof(RGBA));
  
  // create splits
  split(0, _sizeX, 0, _sizeY, 0);
}

void Gear_KDTree::split(int x0, int x1, int y0, int y1, int depth)
{
  if (depth > MAX_DEPTH)
    return;

  if (x1 == x0 || y1 == y0) // *** threshold to set
    return;

  int depthPlusOne = depth+1;

  RGBAint rgba;
  _table->getSum(&rgba, x0, y0, x1, y1);

  int area = _table->getArea(x0, y0, x1, y1);
  int total = SummedAreaTable::total(&rgba);
  int cut = total / 2;
  
  _rasterer->setColor(rgba.R / area, rgba.G / area, rgba.B / area);
  _rasterer->rect(x0, y0, x1, y1, true);
  _rasterer->setColor(0,0,0);
  _rasterer->rect(x0, y0, x1, y1, false);

  // *** pour le moment recherche stupide lineaire poche
  if (depth % 2)
  {
    // vertical split
    int upper = y1;
    int lower = y0;
    int mid;
    while (lower != upper)
    {
      mid = (lower+upper) / 2;
      _table->getSum(&rgba, x0, y0, x1, mid);
      if (SummedAreaTable::total(&rgba) < cut)
        lower = mid+1; // look up //*** attention risque d'erreur : vérifier
      else
        upper = mid;  // look down
    }
    split(x0, x1, y0, mid, depthPlusOne);
    split(x0, x1, mid, y1, depthPlusOne);
  }
  else
  {
    int upper = x1;
    int lower = x0;
    int mid;
    while (lower != upper)
    {
      mid = (lower+upper) / 2;
      _table->getSum(&rgba, x0, y0, mid, y1);
      if (SummedAreaTable::total(&rgba) < cut)
        lower = mid+1; // look right //*** attention risque d'erreur : vérifier
      else
        upper = mid;  // look left
    }
    split(x0, mid, y0, y1, depthPlusOne);
    split(mid, x1, y0, y1, depthPlusOne);
  }
}
