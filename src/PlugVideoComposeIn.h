#ifndef PLUG_VIDEOCOMPOSEIN_INCLUDED
#define PLUG_VIDEOCOMPOSEIN_INCLUDED


#include "PlugVideoCompose.h"


class PlugVideoComposeIn  : public PlugVideoCompose
{
public:    
    PlugVideoComposeIn(Gear* parent, std::string name) : PlugVideoCompose(parent, IN, name){};
    
	
    virtual void init(){};


private:    

};




#endif 
