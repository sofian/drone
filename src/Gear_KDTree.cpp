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
  _AMOUNT_IN = addPlugSignalIn("Depth", 6);
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
  _maxDepth = (int)_AMOUNT_IN->buffer()[0];
  
  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());

  _sizeX = _image->sizeX();
  _sizeY = _image->sizeY();
  _size = _sizeX * _sizeY;

  _data = _image->_data;
  _outData = _outImage->_data;

  _rasterer->setCanvas(_outImage);
  _table->setCanvas(_image);

  // build accumulation buffer
  _table->buildTable();

  // create splits
  split(0, _sizeX-1, 0, _sizeY-1, 0);
}

void Gear_KDTree::split(int x0, int x1, int y0, int y1, int depth)
{

  if (depth > _maxDepth)
    return;

  if (x1 == x0 || y1 == y0) // *** threshold to set
    return;

  // Increment depth by one level.
  depth++;

  // Get the total values in the area.
  RGBAint rgba;
  _table->getSum(&rgba, x0, y0, x1, y1);

  // Useful values.
  int area = _table->getArea(x0, y0, x1, y1);
  int cut = SummedAreaTable::total(&rgba) / 2;

  // Draw a rectangle around the area and paint it with the average color.
  _rasterer->setColor(rgba.R / area, rgba.G / area, rgba.B / area);
  _rasterer->rect(x0, y0, x1, y1, true);
  _rasterer->setColor(0,0,0);
  _rasterer->rect(x0, y0, x1, y1, false);

  // Now split.
  if (depth % 2)
  {
    // vertical split
    int upper = y1;
    int lower = y0;
    int mid = 0;
    // binary search
    while (lower != upper)
    {
      mid = (lower+upper) / 2; // take the mean
      _table->getSum(&rgba, x0, y0, x1, mid);
      if (SummedAreaTable::total(&rgba) < cut)
        lower = mid+1; // look up //*** attention risque d'erreur : vérifier
      else
        upper = mid;  // look down
    }
    // split the area in two
    split(x0, x1, y0, mid, depth);
    split(x0, x1, mid, y1, depth);
  }
  else
  {
    // horizontal split
    int upper = x1;
    int lower = x0;
    int mid = 0;
    // binary search
    while (lower != upper)
    {
      mid = (lower+upper) / 2; // take the mean
      _table->getSum(&rgba, x0, y0, mid, y1);
      if (SummedAreaTable::total(&rgba) < cut)
        lower = mid+1; // look right //*** attention risque d'erreur : vérifier
      else
        upper = mid;  // look left
    }
    // split the area in two
    split(x0, mid, y0, y1, depth);
    split(mid, x1, y0, y1, depth);
  }
}
