#ifndef PLUG_VIDEOIN_INCLUDED
#define PLUG_VIDEOIN_INCLUDED
                           
#include "PlugVideo.h"
#include "Canvas.h"




class PlugVideoIn  : public PlugVideo
{
public:    
    PlugVideoIn(Gear* parent, std::string name);
	virtual ~PlugVideoIn();

    void init();

   
    virtual void onConnection(Plug *plug);
    virtual void onDisconnection(Plug *plug);


private:    
    
};




#endif 
