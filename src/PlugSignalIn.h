#ifndef PLUG_SIGNALIN_INCLUDED
#define PLUG_SIGNALIN_INCLUDED


#include "PlugSignal.h"
              
class PlugSignalIn  : public PlugSignal
{
public:    
    PlugSignalIn(Gear* parent, std::string name, Signal_T default_value=1.0f);
	virtual ~PlugSignalIn();

    virtual void init();
    
    Signal_T* buffer()
    {
        return (connected() ? _Buffer : _DefaultValueBuffer);
    }

    virtual void onConnection(Plug *plug);
    virtual void onDisconnection(Plug *);

    void allocateBuffers();
    void defaultValue(Signal_T default_value);

private:    

    void freeBuffers();
    
    Signal_T *_DefaultValueBuffer;
    Signal_T _DefaultValue;
};




#endif 
