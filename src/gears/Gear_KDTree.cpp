#include "Gear_KDTree.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_KDTree, Gear_KDTree, "KDTree")

Gear_KDTree::Gear_KDTree(Engine *engine, std::string name)
: Gear(engine, "KDTree", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "Depth", new ValueType(6)));
  _rasterer = new Rasterer();
  _table = new SummedAreaTable();
}

Gear_KDTree::~Gear_KDTree()
{
  delete _rasterer;
  delete _table;
}

bool Gear_KDTree::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_KDTree::init()
{
  _rasterer->setImage(_VIDEO_OUT->type());
  _rasterer->setColor(0, 0, 0); // black lines only
  _table->setImage(_VIDEO_IN->type());
}

void Gear_KDTree::runVideo()
{
  // initialize
  _maxDepth = (int)_AMOUNT_IN->type()->value();

  _image = _VIDEO_IN->type();
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  _sizeX = _image->width();
  _sizeY = _image->height();
  _size = _sizeX * _sizeY;

  _data = _image->data();
  _outData = _outImage->data();

  _rasterer->setImage(_outImage);
  _table->setImage(_image);

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
  int area;
  _table->getSum(rgba, area, x0, y0, x1, y1);

  // Useful values.
  //int area = _table->getArea(x0, y0, x1, y1);
  int cut = SummedAreaTable::total(rgba) / 2;

  // Draw a rectangle around the area and paint it with the average color.
  _rasterer->setColor(rgba.r / area, rgba.g / area, rgba.b / area);
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
      _table->getSum(rgba, area, x0, y0, x1, mid); /// XXX pas besoin de "area" ici, il faut du code separe getSumAndArea dans SummedAreaTable...
      if (SummedAreaTable::total(rgba) < cut)
        lower = mid+1; // look up //*** attention risque d'erreur : vérifier
      else
        upper = mid;  // look down
    }
    // split the area in two
    split(x0, x1, y0, mid, depth);
    split(x0, x1, mid, y1, depth);
  } else
  {
    // horizontal split
    int upper = x1;
    int lower = x0;
    int mid = 0;
    // binary search
    while (lower != upper)
    {
      mid = (lower+upper) / 2; // take the mean
      _table->getSum(rgba, area, x0, y0, mid, y1); /// XXX pas besoin de "area" ici, il faut du code separe getSumAndArea dans SummedAreaTable...
      if (SummedAreaTable::total(rgba) < cut)
        lower = mid+1; // look right //*** attention risque d'erreur : vérifier
      else
        upper = mid;  // look left
    }
    // split the area in two
    split(x0, mid, y0, y1, depth);
    split(mid, x1, y0, y1, depth);
  }
}
