#ifndef PLUG_VIDEO_INCLUDED
#define PLUG_VIDEO_INCLUDED
                           
#include "Plug.h"
#include "Canvas.h"


typedef Canvas Video_T;

class PlugVideo  : public Plug
{
public:    
    PlugVideo(Gear* parent, IN_OUT in_out, std::string name) :
        Plug(parent, VIDEO, in_out, name),
        _canvas(NULL){}
    

	virtual ~PlugVideo(){}
    
    Video_T* canvas(){return _canvas;};

protected:
    Video_T *_canvas;
};




#endif 
