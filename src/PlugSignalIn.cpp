#include "PlugSignalIn.h"
#include "PlugSignalOut.h"
#include "Engine.h"


PlugSignalIn::PlugSignalIn(Gear* parent, std::string name, Signal_T default_value) : 
    PlugSignal(parent, IN, name),    
    _DefaultValueBuffer(NULL),
    _DefaultValue(default_value)

{
    allocateBuffers();
}

PlugSignalIn::~PlugSignalIn()
{
    freeBuffers();
}

void PlugSignalIn::init()
{
}

void PlugSignalIn::freeBuffers()
{
    if (_DefaultValueBuffer != NULL)
        delete [] _DefaultValueBuffer;
}

void PlugSignalIn::allocateBuffers()
{
    freeBuffers();

    _DefaultValueBuffer = new Signal_T[Engine::signalInfo().blockSize()];        
    defaultValue(_DefaultValue);
}

void PlugSignalIn::defaultValue(Signal_T default_value)
{
    _DefaultValue = default_value;
            
    std::fill(_DefaultValueBuffer, &_DefaultValueBuffer[Engine::signalInfo().blockSize()], _DefaultValue);    
}

void PlugSignalIn::onConnection(Plug *plug)
{
    //plug.connect certify that plug is of type PlugSignalOut   
     _Buffer = ((PlugSignal*)plug)->buffer();
}

void PlugSignalIn::onDisconnection(Plug *)
{
    _Buffer = NULL;
}
