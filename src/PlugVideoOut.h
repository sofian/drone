#ifndef PLUG_VIDEOOUT_INCLUDED
#define PLUG_VIDEOOUT_INCLUDED
                           
#include "PlugVideo.h"
#include "Canvas.h"

class PlugVideoOut  : public PlugVideo
{
public:    
    PlugVideoOut(Gear* parent, std::string name);
	virtual ~PlugVideoOut();

    void init();
    

};




#endif 
