#include "PlugSignalOut.h"
#include "PlugSignalIn.h"
#include "Engine.h"


PlugSignalOut::PlugSignalOut(Gear* parent, std::string name) : 
    PlugSignal(parent, OUT, name)    
{
    allocateBuffers();
}

PlugSignalOut::~PlugSignalOut()
{
    freeBuffers();
}

void PlugSignalOut::init()
{
}

void PlugSignalOut::freeBuffers()
{
    if (_Buffer != NULL)
        delete [] _Buffer;
}

void PlugSignalOut::allocateBuffers()
{
    freeBuffers();
 
    _Buffer = new Signal_T[Engine::signalInfo().blockSize()];        
    std::fill(_Buffer, &_Buffer[Engine::signalInfo().blockSize()], 0.0f);
}


