// PlugVideo.cpp: implementation of the PlugVideo class.
//
//////////////////////////////////////////////////////////////////////

#include "PlugVideoOut.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


PlugVideoOut::PlugVideoOut(Gear* parent, std::string name) : 
    PlugVideo(parent, OUT, name)
{
    _canvas = new Video_T;
}

PlugVideoOut::~PlugVideoOut()
{
    delete _canvas;
}

void PlugVideoOut::init()
{

}

