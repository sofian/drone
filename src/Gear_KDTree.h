#ifndef GEAR_KDTREE_INCLUDED
#define GEAR_KDTREE_INCLUDED

#include "Gear.h"
#include <assert.h>
#include "Timing.h"
#include "Rasterer.h"
#include "SummedAreaTable.h"

#define MAX_DEPTH 10
//class PlugSignal;

class Gear_KDTree : public Gear  
{
  struct KDNode
  {
    int _x0, _x1, _y0, _y1;
    KDNode *_left, *_right;
    KDNode(int x0, int x1, int y0, int y1)
      : _x0(x0), _x1(x1), _y0(y0), _y1(y1), _left(0), _right(0)
    {}
  };
  
public:
    Gear_KDTree(Engine *engine, std::string name);
    virtual ~Gear_KDTree();

    void runVideo();

    PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
    PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};
  //    PlugSignalIn* AMOUNT_IN(){return _AMOUNT_IN;};

    bool ready();

private:

    PlugVideoIn *_VIDEO_IN;
    PlugVideoOut *_VIDEO_OUT;
  //    PlugSignalIn *_AMOUNT_IN;

    //local var
    Video_T *_image; 
    Video_T *_outImage; 
    RGBA *_data;
    RGBA *_outData;

  SummedAreaTable *_table;
  
    int _sizeX;
    int _sizeY;
    int _size;
  
    int _r;
    int _g;
    int _b;
    
    unsigned char *_imageIn;
    unsigned char *_imageOut;

    int _x1,_y1,_x2,_y2;

    Rasterer *_rasterer;

  void init();
  int accum(int x0, int x1, int y0, int y1);
  
  void split(KDNode *node, int depth);
  void del(KDNode *node);

};

#endif 
