#ifndef VIDEOOUTPUTX11BASE_INCLUDED
#define VIDEOOUTPUTX11BASE_INCLUDED

extern "C"{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/xf86vmode.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <inttypes.h>
}

// Clean up the mess caused by the X headers
/* #undef Bool       */
/* #undef Above      */
/* #undef Below      */
/* #undef KeyPress   */
/* #undef KeyRelease */
/* #undef FocusOut   */
/* #undef FocusIn    */
//#undef None

struct XVisualInfo_fixed
{
    Visual *visual;
    VisualID visualid;
    int screen;
    unsigned int depth;
    int class_fixed;
    unsigned long red_mask;
    unsigned long green_mask;
    unsigned long blue_mask;
    int colormap_size;
    int bits_per_rgb;
};


#include "VideoOutput.h"

class Canvas;

class VideoOutputX11Base : public VideoOutput
{
public:
    VideoOutputX11Base();
    virtual ~VideoOutputX11Base();

protected:

    bool togglefullscreen(bool fs, int xRes, int yRes);
    bool openXDisplay();
    bool checkXShmExtension();
    bool createXWindow(int xRes, int yRes);
    void destroyXWindow();
    bool mapWindow();
    bool unMapWindow();
    XShmSegmentInfo *createShmSegment(int size);//assign segment to _shmInfo and return the segment info
    void destroyShm();
    bool createGC();
    void processX11Events();
    virtual void onResize(int , int ){}
    void resizeWindow(int sizeX, int sizeY);
    
    
    void* _display;//! handle on the X11 display
    Window _window;//! handle on the X11 window
    XF86VidModeModeInfo _desktopMode;//! The Desktop videomode
    XVisualInfo _visualInfo;

    XShmSegmentInfo _shmInfo;//! the current used shm segment
    GC _gc;
    int _currentWindowWidth, _currentWindowHeight;

};

#endif
