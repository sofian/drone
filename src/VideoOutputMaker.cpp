// VideoOutputMaker.cpp: implementation of the VideoOutputMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "VideoOutputMaker.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

std::map<std::string, VideoOutputMaker*> *VideoOutputMaker::_Registry;

VideoOutputMaker::VideoOutputMaker(std::string type) 
{
    static bool registry_instanciated=false;

    if (!registry_instanciated)
    {           
        _Registry = new std::map<std::string, VideoOutputMaker*>();
        registry_instanciated=true;
    }
    
    _Registry->insert(std::make_pair(type, this));
}

VideoOutput* VideoOutputMaker::makeVideoOutput(std::string type)
{    
    VideoOutputMaker *VideoOutputmaker= _Registry->find(type)->second;

    //VideoOutput non trouve dans le registry
    assert(VideoOutputmaker != NULL);

    return VideoOutputmaker->internalMakeVideoOutput();
}

void VideoOutputMaker::getAllVideoOutputsType(std::vector<std::string> &videoOutputsType)
{
    for (std::map<std::string, VideoOutputMaker*>::iterator it=_Registry->begin(); it != _Registry->end(); ++it)
    {
        videoOutputsType.push_back(it->first);
    }
}
