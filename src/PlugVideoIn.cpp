2// PlugVideo.cpp: implementation of the PlugVideo class.
//
//////////////////////////////////////////////////////////////////////

#include "PlugVideoIn.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


PlugVideoIn::PlugVideoIn(Gear* parent, std::string name) : 
    PlugVideo(parent, IN, name)    
{
}

PlugVideoIn::~PlugVideoIn()
{    
}

void PlugVideoIn::init()
{

}

void PlugVideoIn::onConnection(Plug *plug)
{
    //typecast ok, le type a ete verifie dans plug::connect
    _canvas = ((PlugVideo*)plug)->canvas();
}

void PlugVideoIn::onDisconnection(Plug *)
{
    _canvas = NULL;
}
