/* Inspired from the xvvideowindow class of jenskamenik@web.de Copyright (c) 2001, Jens Kamenik */

#include "VideoOutputX11Base.h"


#include <iostream>

using namespace X11;

VideoOutputX11Base::VideoOutputX11Base() :
VideoOutput(),
_display(NULL),
_window(0)
{
  memset(&_shmInfo, 0, sizeof(XShmSegmentInfo));
  _shmInfo.shmid=-1;
}

VideoOutputX11Base::~VideoOutputX11Base()
{    
}


bool VideoOutputX11Base::togglefullscreen(bool fs, int xRes, int yRes)
{
  std::cout << "switching to fullscreen..." << std::endl;

  std::cout << "finding best fit resolution..." << std::endl;

  XF86VidModeModeInfo **modes;
  int nbmodes=0;
  int bestmode=0;

  XF86VidModeGetAllModeLines((Display*)_display, DefaultScreen(_display), &nbmodes, &modes);

  //save desktop-resolution
  _desktopMode = *modes[0];

  for (int i=0; i < nbmodes; i++)
  {
    if ((modes[i]->hdisplay == xRes) && (modes[i]->vdisplay == yRes))
    {
      bestmode = i;
    }
  }



  XF86VidModeSwitchToMode((Display*)_display, DefaultScreen(_display), modes[bestmode]);
  XF86VidModeSetViewPort((Display*)_display, DefaultScreen(_display), 0, 0);

  XWarpPointer((Display*)_display, None, _window, 0, 0, 0, 0, 0, 0);

  //free the VideoModes Array
  XFree(modes);

  return true;
}

bool VideoOutputX11Base::openXDisplay()
{
  std::cout << "opening the X display...";

  if ((_display=(void*)XOpenDisplay(0))==NULL)
  {
    std::cout << "FAIL!" << std::endl;
    return false;
  }

  std::cout << "done" << std::endl;
  return true;
}

bool VideoOutputX11Base::checkXShmExtension()
{
  std::cout << "checking for XShm extension...";

  int major=0, minor=0;
  int pixmaps=0;

  int result=XShmQueryVersion ((Display*)_display, &major, &minor, &pixmaps);
  if ((result == 0) || (major < 1) || ((major == 1) && (minor < 1)))
  {
    std::cout  << "no xshm extension!" << std::endl;
    return false;
  }

  std::cout << "ok" << std::endl;
  return true;
}

bool VideoOutputX11Base::createXWindow(int xRes, int yRes)
{
  std::cout << "- XWindow create -" << std::endl;

  if (_display==NULL)
  {
    std::cout << "FAIL! no display" << std::endl;        
    return false;
  }

  _xRes=xRes;
  _yRes=yRes;

  int screen = DefaultScreen(_display);

  std::cout << "selecting visual info with highest depth...";

  if (!findHighestDepthVisual(_visualInfo))
  {
    std::cout << "FAIL! no truecolor mode" << std::endl;        
    return false;
  }
  _bpp = _visualInfo.depth;

  std::cout << "done (" << _bpp << ")" << std::endl;;


  //now build window attributes
  XSetWindowAttributes windowAttributes;
  windowAttributes.background_pixmap = None;
  windowAttributes.backing_store = NotUseful;
  windowAttributes.border_pixel = 0;
  windowAttributes.event_mask = 0;
  windowAttributes.colormap = XCreateColormap ((Display*)_display, RootWindow ((Display*)_display, _visualInfo.screen), _visualInfo.visual, AllocNone);


  std::cout << "creating the Window...";

  _window = XCreateWindow((Display*)_display, RootWindow((Display*)_display, _visualInfo.screen),
                          0, 0, xRes, yRes, 0, _visualInfo.depth, InputOutput, _visualInfo.visual,
                          CWBorderPixel | CWColormap | CWEventMask | CWBackPixmap | CWBackingStore , &windowAttributes);

  if (_window==0)
  {
    std::cout << "FAIL!" << std::endl;
    return false;
  }

  XSelectInput((Display*)_display, _window, StructureNotifyMask);
  XSetStandardProperties((Display*)_display, _window, "Drone video output", "Drone video output", None, NULL, 0, NULL);            
  //XGrabPointer((Display*)_display, _window, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, _window, None, CurrentTime);        
  //XMapRaised((Display*)_display, _window);
  //XGrabKeyboard((Display*)_display, _window, True, GrabModeAsync, GrabModeAsync, CurrentTime);   

  std::cout << "done" << std::endl;

  mapWindow();

  return true;
}

void VideoOutputX11Base::destroyXWindow()
{
  std::cout << "- XWindow destroy -" << std::endl;

  unMapWindow();

  std::cout << "destroying window...";

  XDestroyWindow((Display*)_display, _window);
  _window=0;

  std::cout << "done" << std::endl;
}       

bool VideoOutputX11Base::mapWindow()
{
  std::cout << "mapping window...";
  if (_display==NULL)
  {
    std::cout << "FAIL! no display" << std::endl;
    return false;
  }

  if (_window==0)
  {
    std::cout << "FAIL! no window" << std::endl;
    return false;
  }

  XMapWindow((Display*)_display, _window);
  XEvent event;
  do
  {
    XMaskEvent((Display*)_display, StructureNotifyMask, &event);
  } while ( (event.type != MapNotify) || (event.xmap.event != _window) );

  std::cout << "done" << std::endl;
  return true;
}

bool VideoOutputX11Base::unMapWindow()
{
  std::cout << "unMapping window...";
  if (_display==NULL)
  {
    std::cout << "FAIL! no display" << std::endl;
    return false;
  }

  if (_window==0)
  {
    std::cout << "FAIL! no window" << std::endl;
    return false;
  }

  XUnmapWindow((Display*)_display, _window);    
  XEvent event;
  do
  {
    XMaskEvent((Display*)_display, StructureNotifyMask, &event);
  } while ( (event.type != UnmapNotify) || (event.xunmap.event != _window) );

  std::cout << "done" << std::endl;
  return true;
}


XShmSegmentInfo *VideoOutputX11Base::createShmSegment(int size)
{ 
  if (_display==NULL)
    return NULL;


  destroyShm();

  _shmInfo.shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0777);

  if (_shmInfo.shmid == -1)
  {
    std::cout << "no shm" << std::endl;
    return NULL;
  }
  
  _shmInfo.shmaddr = (char*)shmat(_shmInfo.shmid, 0, 0777);

  XSync((Display*)_display, False);

  if (_shmInfo.shmaddr == (char*)-1)
  {
    std::cout << "no shm addr" << std::endl;
    _shmInfo.shmaddr=NULL;
    return NULL;
  }


  _shmInfo.readOnly = False;
  XShmAttach((Display*)_display, &_shmInfo);

  XSync((Display*)_display, False);

  return &_shmInfo;
}

bool VideoOutputX11Base::createGC()
{
  std::cout << "creating gc...";

  if (_display==NULL)
  {
    std::cout << "FAIL! no display" << std::endl;
    return false;
  }

  if (_window==0)
  {
    std::cout << "FAIL! no window" << std::endl;
    return false;
  }

  XGCValues gcValues;
  _gc = XCreateGC((Display*)_display, _window, 0, &gcValues);

  std::cout << "done" << std::endl;

  return true;
}

void VideoOutputX11Base::destroyShm()
{    
  if (_shmInfo.shmid==-1)
    return;

  XShmDetach((Display*)_display, &_shmInfo);

  XSync ((Display*)_display, False);
   
  if (_shmInfo.shmaddr!=NULL)
  {
    shmdt(_shmInfo.shmaddr);
    _shmInfo.shmaddr=NULL;    
  }

  shmctl(_shmInfo.shmid, IPC_RMID, 0);
  _shmInfo.shmid=-1;
  
  XSync ((Display*)_display, False);

}

void VideoOutputX11Base::processX11Events()
{
  XEvent event;

  while (XPending((Display*)_display) > 0)
  {
    XNextEvent((Display*)_display, &event);
    switch (event.type)
    {
    case ConfigureNotify:
      if (_xRes!=event.xconfigure.width || _yRes!=event.xconfigure.height)
      {
        _xRes = event.xconfigure.width;
        _yRes = event.xconfigure.height;
        onResize(_xRes, _yRes);
      }
      break;    
    }
  }
}

void VideoOutputX11Base::resizeWindow(int sizeX, int sizeY)    
{
  _xRes=sizeX;
  _yRes=sizeY;
  XWindowChanges windowChanges;
  windowChanges.width = sizeX;
  windowChanges.height = sizeY;
  XConfigureWindow((Display*)_display, _window, CWWidth | CWHeight, &windowChanges);
  XSync((Display*)_display, False);
}


bool VideoOutputX11Base::findHighestDepthVisual(XVisualInfo &visualInfo)
{
  if (_display==NULL)
    return false;

  int screen = DefaultScreen(_display);

  XVisualInfo_fixed vTfixed;
  XVisualInfo visualInfoTemplate;

  if (sizeof(XVisualInfo_fixed) != sizeof(XVisualInfo) )
    return false;

  vTfixed.visual=visualInfoTemplate.visual;
  vTfixed.visualid=visualInfoTemplate.visualid;
  vTfixed.screen=visualInfoTemplate.screen;
  vTfixed.depth=visualInfoTemplate.depth;
  vTfixed.red_mask=visualInfoTemplate.red_mask;
  vTfixed.green_mask= visualInfoTemplate.green_mask;
  vTfixed.blue_mask= visualInfoTemplate.blue_mask;
  vTfixed.colormap_size= visualInfoTemplate.colormap_size;
  vTfixed.bits_per_rgb=  visualInfoTemplate.bits_per_rgb;    
  vTfixed.class_fixed=TrueColor;

  memcpy((void*)&visualInfoTemplate,(void*)&vTfixed,sizeof(XVisualInfo));

  visualInfoTemplate.screen = screen;

  int nbVisualInfo=0;
  XVisualInfo *visualInfoTable = XGetVisualInfo ((Display*)_display, VisualScreenMask | VisualClassMask, &visualInfoTemplate, &nbVisualInfo);

  if (visualInfoTable == NULL)
    return false;

  // find the visual with the highest depth
  XVisualInfo* tmpVisualInfo = visualInfoTable;
  for (int i = 1; i < nbVisualInfo; i++)
    if (visualInfoTable[i].depth > tmpVisualInfo->depth)
      tmpVisualInfo = visualInfoTable + i;

    //keep the selected visualinfo before freeing the table
  visualInfo = *tmpVisualInfo;
  XFree(visualInfoTable);

  return true;
}

void VideoOutputX11Base::waitForShmCompletion()
{
  XEvent event;

  while (XPending((Display*)_display) > 0)
  {
    XNextEvent((Display*)_display, &event);
     

    if (event.type == XShmGetEventBase((Display*)_display) + ShmCompletion)
    {
      std::cout << "ShmCompletion" << std::endl; 
      return;   
    }


  }

  return;
}
