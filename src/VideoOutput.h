#ifndef VIDEOOUTPUT_INCLUDED
#define VIDEOOUTPUT_INCLUDED

class Canvas;

class VideoOutput
{
public:
    VideoOutput() : _xRes(0), _yRes(0) {}
    virtual ~VideoOutput(){}

    virtual bool init(int xRes, int yRes, int bpp, bool fullscreen)=0;    
    virtual void render(Canvas &canvas)=0;
    virtual void fullscreen(bool fs)=0;
    virtual void destroy()=0;

protected:
    int _xRes, _yRes;
};

      
#endif

