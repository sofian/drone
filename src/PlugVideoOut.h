#ifndef PLUG_VIDEOOUT_INCLUDED
#define PLUG_VIDEOOUT_INCLUDED
                           
#include "PlugVideo.h"
#include "Canvas.h"

template <class T>
class PlugOut  : public Plug<T>
{
public:
  PlugOut(Gear* parent, std::string name);
  virtual ~PlugOut();

  void init();
};




#endif 
