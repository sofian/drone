#ifndef GEAR_VIDEOOUTPUT_INCLUDED
#define GEAR_VIDEOOUTPUT_INCLUDED

#include "Gear.h"





#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <X11/extensions/xf86vmode.h>
                         
class PlugVideo;

class Gear_VideoOutput : public Gear  
{
public:
	
    
    Gear_VideoOutput(Engine *engine, std::string name);
	virtual ~Gear_VideoOutput();

    
    void setVideoResolution(int xres, int yres, int bpp, bool fullscreen);

    void prePlay();
    void postPlay();
    void init();
    void runVideo();
    

    PlugVideoComposeIn* VCOMPOSE_IN(){return _VCOMPOSE_IN;};
    
    bool ready();

private:
    static const int DEFAULT_XRES;
    static const int DEFAULT_YRES;
    static const int DEFAULT_BPP;    

    int createDisplayWindow(int &xres, int &yres, int &bpp, bool &fullscreen);
    int initGL(int xres, int yres);
    void logGLSpecs();
    void swapBuffers();
    void destroyDisplayWindow(bool fullscreen);

    PlugVideoComposeIn *_VCOMPOSE_IN;

    int _XRes, _YRes;
    int _BPP;
    bool _FullScreen;


/*
    GLX
*/
    void* _DisplayWindow;
    int _XScreen;//!> Handle on the XScreen
    GLXContext _XGLXContext;
    XSetWindowAttributes _XWindowAttributes;    
    XF86VidModeModeInfo _XDesktopMode;//!> The Desktop videomode
    long unsigned int _XWin;


};

#endif
