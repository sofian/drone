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

  _accumbuf = NULL;  
}

Gear_KDTree::~Gear_KDTree()
{
  delete _rasterer;
}

void Gear_KDTree::init()
{
  _rasterer->setCanvas(_VIDEO_OUT->canvas());
  _rasterer->setColor(0, 0, 0); // black lines only
}


bool Gear_KDTree::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_KDTree::runVideo()
{
  int  *accumptrpred;
  // initialize
  
  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());

  _rasterer->setCanvas(_outImage);

  _sizeX = _image->sizeX();
  _sizeY = _image->sizeY();
  _size = _sizeX * _sizeY;

  if(_accumbuf == NULL) 
    _accumbuf = new int[_size];
  
   memset(_accumbuf, 0, _size * sizeof(int));
  
  _data = _image->_data;
  _outData = _outImage->_data;

  _imageIn = (unsigned char*)_data;

  // build accumulation buffer
    
  _tempAccumbuf = _accumbuf;
  
  for(int y=0;y<_sizeY;y++)
  {
    _acc = 0;
    if (y == 0)
      for(int x=0;x<_sizeX;x++)
      {
        _acc += *(_imageIn++);
        _acc += *(_imageIn++);
        _acc += *(_imageIn++);
        _imageIn++;
        
        *(_tempAccumbuf++) += _acc;
      }
    else
    {
      accumptrpred = _tempAccumbuf - _sizeX;
      for(int x=0;x<_sizeX;x++)
      {
        _acc += *(_imageIn++);
        _acc += *(_imageIn++);
        _acc += *(_imageIn++);
        _imageIn++;
        
        *(_tempAccumbuf++ ) += _acc + *(accumptrpred++);
      }
    }
  }

  _imageIn = (unsigned char*)_data;
  _imageOut = (unsigned char*)_outData;
  
  memcpy(_imageOut, _imageIn, _size * 4 * sizeof(unsigned char));
  
  // create splits
  KDNode *root = new KDNode(0, _sizeX, 0, _sizeY);

  //std::cout << "=====" << std::endl;
  split(root, 0);
  del(root);
  
}

int Gear_KDTree::accum(int x0, int x1, int y0, int y1)
{
  if(x0 < 0) x0 = 0;
  if(x1 < 0) x1 = 0;
  if(y0 < 0) y0 = 0;
  if(y1 < 0) y1 = 0;
  
  if(x0 >= _sizeX) x0 = _sizeX-1;
  if(x1 >= _sizeX) x1 = _sizeX-1;
  if(y0 >= _sizeY) y0 = _sizeY-1;
  if(y1 >= _sizeY) y1 = _sizeY-1;

  return
    _accumbuf[y1 * _sizeX + x1] -
    _accumbuf[y0 * _sizeX + x1] -
    _accumbuf[y1 * _sizeX + x0] +
    _accumbuf[y0 * _sizeX + x0];
}

void Gear_KDTree::split(Gear_KDTree::KDNode *node, int depth)
{
  if (!node || depth > MAX_DEPTH)
    return;

  if (node->_x1 == node->_x0 || node->_y1 == node->_y0)
    return;

  //std::cout << node->_x0 << "," <<  node->_x1 <<","<< node->_y0<<"," <<node->_y1 << std::endl;
  
  _tempAccumbuf = _accumbuf;

  int total = accum(node->_x0, node->_x1, node->_y0, node->_y1);
  int cut = total / 2;
  int gray = total / ((node->_x1-node->_x0) * (node->_y1-node->_y0) * 3);
  _rasterer->setColor(gray, gray, gray);
  _rasterer->rect(node->_x0, node->_y0, node->_x1, node->_y1, true);
  _rasterer->setColor(0,0,0);
  _rasterer->rect(node->_x0, node->_y0, node->_x1, node->_y1, false);

  // *** pour le moment recherche stupide lineaire poche
  // *** fucking unefficient with all these "new" 
  if (depth % 2) 
    for(int y=node->_y0;y<node->_y1; ++y)
    {
      if (accum(node->_x0, node->_x1, node->_y0, y) >= cut)
      {
        if (!node->_left)
          node->_left = new KDNode(node->_x0, node->_x1, node->_y0, y);
        else
          *(node->_left) = KDNode(node->_x0, node->_x1, node->_y0, y);
        if (!node->_right)
          node->_right = new KDNode(node->_x0, node->_x1, y, node->_y1);
        else
          *(node->_right) = KDNode(node->_x0, node->_x1, y, node->_y1);
        break;
      }
    }
  else
    for(int x=node->_x0;x<node->_x1; ++x)
    {
      if (accum(node->_x0, x, node->_y0, node->_y1) >= cut)
      {
        if (!node->_left)
          node->_left = new KDNode(node->_x0, x, node->_y0, node->_y1);
        else
          *(node->_left) = KDNode(node->_x0, x, node->_y0, node->_y1);
        if (!node->_right)
          node->_right = new KDNode(x, node->_x1, node->_y0, node->_y1);
        else
          *(node->_right) = KDNode(x, node->_x1, node->_y0, node->_y1);
        break;
      }
    }
  split(node->_left, depth+1);
  split(node->_right, depth+1);
}

void Gear_KDTree::del(Gear_KDTree::KDNode *node)
{
  if (node)
  {
    del(node->_left);
    del(node->_right);
    delete node;
  }
}
