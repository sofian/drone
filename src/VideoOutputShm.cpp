#include "VideoOutputShm.h"
#include "VideoOutputMaker.h"

#include <iostream>
#include "Canvas.h"

Register_VideoOutput(MAKERVideoOutputShm, VideoOutputShm, "Shm")

VideoOutputShm::VideoOutputShm() :
    VideoOutputX11Base(),
    _xImage(NULL),
    _frameSizeX(0),
    _frameSizeY(0),
    _frameSize(0)
{    
}

VideoOutputShm::~VideoOutputShm()
{
    destroy();
}

void VideoOutputShm::destroy()
{
    destroyXWindow();    

    destroyXImage();

    destroyShm();
}

void VideoOutputShm::fullscreen(bool fs)
{
    togglefullscreen(fs, _xRes, _yRes);
}

void VideoOutputShm::render(Canvas &canvas)
{    
    processX11Events();

    //only recreate xvimage if framesize change
    if (_frameSizeX!=canvas.sizeX() || _frameSizeY!=canvas.sizeY())
    {
        _frameSizeX=canvas.sizeX();
        _frameSizeY=canvas.sizeY();
        _frameSize=_frameSizeX*_frameSizeY;
        createXImage(_frameSizeX, _frameSizeY);
    }

    unsigned char *xData = (unsigned char*)_xImage->data;
    unsigned char *data = (unsigned char*)canvas._data;
    for (int i=0;i<_frameSize;++i)    
    {
        *xData++ = *(data+2);
        *xData++ = *(data+1);
        *xData++ = *(data);
        *xData++;
        data+=4;
    }
    
    XShmPutImage((Display*)_display, _window, _gc, _xImage, 0, 0, 0, 0, _frameSizeX, _frameSizeY, False);
    
    XFlush((Display*)_display);
}

bool VideoOutputShm::init(int xRes, int yRes, int bpp, bool fullscreen)
{            
    std::cout << "--==|| Xv output initialization ||==--" << std::endl;
    
    _xRes = xRes;
    _yRes = yRes;

    if (!openXDisplay())
        return false;
    
    if (!checkXShmExtension())
        return false;
    
    if (!createXWindow(xRes, yRes))
        return false;

    if (!createGC())
        return false;
    
    return true;
}

XImage* VideoOutputShm::createXImage(int sizeX, int sizeY)
{    
    destroyXImage();
    
    //we will create a ZPixmap format image -> 32bit
    int size = 4 * sizeX * sizeY;

    XShmSegmentInfo *shmInfo= createShmSegment(size);
        
    if (shmInfo==NULL)
        return NULL;

    if (shmInfo->shmaddr==NULL)
        return NULL;
            
    _xImage=XShmCreateImage((Display*)_display, _visualInfo.visual, _visualInfo.depth, ZPixmap, NULL, shmInfo, sizeX, sizeY);
    _xImage->data = shmInfo->shmaddr;    

    resizeWindow(sizeX, sizeY);

    return _xImage;
}

void VideoOutputShm::destroyXImage()
{
    if (_xImage != NULL)
    {
        XFree(_xImage);
        _xImage = NULL;
    };
}

void VideoOutputShm::onResize(int sizeX, int sizeY)
{
    //force size to be of frame size
    resizeWindow(_frameSizeX, _frameSizeY);
}
