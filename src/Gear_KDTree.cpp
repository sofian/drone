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

int Gear_KDTree::accum(int x0, int x1, int y0, int y1)
{
  int rgba[4];
  _table->getSum((RGBAint*)rgba, x0, y0, x1, y1);
  return ( *(rgba) + *(rgba+1) + *(rgba+2) );
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
  int total = rgba.R + rgba.G + rgba.B; //*(rgba) + *(rgba+1) + *(rgba+2);
  int cut = total / 2;
  
  _rasterer->setColor(rgba.R / area, rgba.G / area, rgba.B / area);
  _rasterer->rect(x0, y0, x1, y1, true);
  _rasterer->setColor(0,0,0);
  _rasterer->rect(x0, y0, x1, y1, false);

  // *** pour le moment recherche stupide lineaire poche
  // *** fucking unefficient with all these "new" 
  if (depth % 2)
    // vertical split
    for(int y=y0;y<y1; ++y)
    {
      if (accum(x0, x1, y0, y) >= cut)
      {
        split(x0, x1, y0, y, depthPlusOne);
        split(x0, x1, y, y1, depthPlusOne);
        break;
      }
    }
  else
    // horizontal split
    for(int x=x0;x<x1; ++x)
    {
      if (accum(x0, x, y0, y1) >= cut)
      {
        split(x0, x, y0, y1, depthPlusOne);
        split(x, x1, y0, y1, depthPlusOne);
        break;
      }
    }
}
