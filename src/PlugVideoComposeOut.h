#ifndef PLUG_VIDEOCOMPOSEOUT_INCLUDED
#define PLUG_VIDEOCOMPOSEOUT_INCLUDED


#include "PlugVideoCompose.h"


class PlugVideoComposeOut  : public PlugVideoCompose
{
public:    
    PlugVideoComposeOut(Gear* parent, std::string name) : PlugVideoCompose(parent, OUT, name){};
    
	
    virtual void init(){};


private:    

};




#endif 
