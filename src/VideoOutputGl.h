#ifndef VIDEOOUTPUTGL_INCLUDED
#define VIDEOOUTPUTGL_INCLUDED

#include "VideoOutputX11Base.h"

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <X11/extensions/xf86vmode.h>

class Canvas;

class VideoOutputGl : public VideoOutputX11Base
{
public:
    VideoOutputGl();
    ~VideoOutputGl();

    void fullscreen(bool fs);
    void destroy();
    bool init(int xRes, int yRes, int bpp, bool fullscreen);
    void render(Canvas &canvas);
    

protected:
    
    void onResize(int sizeX, int sizeY);
    bool createGLXContext();
    int initGl(int xRes, int yRes);    

private:
                      
    GLXContext _XGLXContext;
    int _frameSizeX;
    int _frameSizeY;
    int _frameSize;
    bool _glInitialized;
    float _texSizeX, _texSizeY;
    
};

#endif
