#ifndef PLUG_SIGNAL_INCLUDED
#define PLUG_SIGNAL_INCLUDED


#include "Plug.h"

#include "Signal.h"

              
class PlugSignal  : public Plug
{
public:    
    PlugSignal(Gear* parent, IN_OUT in_out, std::string name) :
        Plug(parent, SIGNAL, in_out, name),
        _Buffer(NULL){}
    

    virtual Signal_T* buffer()=0;
    virtual void allocateBuffers()=0;
	
    virtual ~PlugSignal(){};

protected:    

    Signal_T *_Buffer;
};




#endif 
