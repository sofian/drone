#ifndef PLUG_SIGNALOUT_INCLUDED
#define PLUG_SIGNALOUT_INCLUDED


#include "PlugSignal.h"
              
class PlugSignalOut  : public PlugSignal
{
public:    
    PlugSignalOut(Gear* parent, std::string name);
	virtual ~PlugSignalOut();

    virtual void init();
    
    Signal_T* buffer()
    {
        return _Buffer;
    }

    void allocateBuffers();

private:    

    void freeBuffers();
    
};




#endif 
