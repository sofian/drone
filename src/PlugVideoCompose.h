#ifndef PLUG_VIDEOCOMPOSE_INCLUDED
#define PLUG_VIDEOCOMPOSE_INCLUDED


#include "Plug.h"


class PlugVideoCompose  : public Plug
{
public:    
    PlugVideoCompose(Gear* parent, IN_OUT in_out, std::string name) : Plug(parent, VIDEO_COMPOSE, in_out, name){};
    	
    virtual void init(){};


private:    

};




#endif 
