#include "Gear_VideoOutput.h"

#include "GearMaker.h"
#include "Engine.h"

#include <iostream>

Register_Gear(MAKERGear_VideoOutput, Gear_VideoOutput, "VideoOutput")


const int Gear_VideoOutput::DEFAULT_XRES = 512;
const int Gear_VideoOutput::DEFAULT_YRES = 512;
const int Gear_VideoOutput::DEFAULT_BPP = 24;    

static int attrListSgl16[] = {GLX_RGBA, GLX_RED_SIZE, 5, 
    GLX_GREEN_SIZE, 5, 
    GLX_BLUE_SIZE, 5, 
    GLX_DEPTH_SIZE, 16,
    None};

static int attrListDbl16[] = { GLX_RGBA, GLX_DOUBLEBUFFER, 
    GLX_RED_SIZE, 5, 
    GLX_GREEN_SIZE, 5, 
    GLX_BLUE_SIZE, 5, 
    GLX_DEPTH_SIZE, 16,
    None};

static int attrListSgl24[] = {GLX_RGBA, GLX_RED_SIZE, 6, 
    GLX_GREEN_SIZE, 6, 
    GLX_BLUE_SIZE, 6, 
    GLX_DEPTH_SIZE, 24,
    None};

static int attrListDbl24[] = { GLX_RGBA, GLX_DOUBLEBUFFER, 
    GLX_RED_SIZE, 6, 
    GLX_GREEN_SIZE, 6, 
    GLX_BLUE_SIZE, 6, 
    GLX_DEPTH_SIZE, 24,
    None};

static int attrListSgl32[] = {GLX_RGBA, GLX_RED_SIZE, 8, 
    GLX_GREEN_SIZE, 8, 
    GLX_BLUE_SIZE, 8, 
    GLX_DEPTH_SIZE, 32,
    None};

static int attrListDbl32[] = { GLX_RGBA, GLX_DOUBLEBUFFER, 
    GLX_RED_SIZE, 8, 
    GLX_GREEN_SIZE, 8, 
    GLX_BLUE_SIZE, 8, 
    GLX_DEPTH_SIZE, 32,
    None};



Gear_VideoOutput::Gear_VideoOutput(Engine *engine, std::string name) : Gear(engine, "VideoOutput", name),
    _XRes(DEFAULT_XRES),
    _YRes(DEFAULT_YRES),
    _BPP(DEFAULT_BPP),
    _FullScreen(false),
    _DisplayWindow(NULL),
    _XGLXContext(NULL)
      
{
    _VCOMPOSE_IN = addPlugVideoComposeIn(name);
}

Gear_VideoOutput::~Gear_VideoOutput()
{
    destroyDisplayWindow(_FullScreen);
}

bool Gear_VideoOutput::ready()
{
    return _VCOMPOSE_IN->connected();
}

void Gear_VideoOutput::init()
{    
    std::cout << "Initializing VideoOutput..." << std::endl;

    destroyDisplayWindow(_FullScreen);
    createDisplayWindow(_XRes, _YRes, _BPP, _FullScreen);
       
    std::cout << "Resolution : " << _XRes << "x" << _YRes << "x" << _BPP << std::endl;
    std::cout << "Initializing VideoOutput...done" << std::endl;
    
}

void Gear_VideoOutput::prePlay()
{
    initGL(_XRes,_YRes);
}

void Gear_VideoOutput::postPlay()
{
    //release the context
    glXMakeCurrent((Display*)_DisplayWindow, None, NULL);    
}

void Gear_VideoOutput::runVideo()
{       
    if (!_VCOMPOSE_IN->connected())
        return;
            
    swapBuffers();
    glClear(GL_COLOR_BUFFER_BIT);
}

void Gear_VideoOutput::setVideoResolution(int xres, int yres, int bpp, bool fullscreen)
{
    _XRes=xres;
    _YRes=yres;
    _BPP=bpp;
    _FullScreen=fullscreen;

    init();
}

/*
    GLX
*/

int Gear_VideoOutput::createDisplayWindow(int &xres, int &yres, int &bpp, bool &fullscreen)
{
    std::cout << "opening the Display Window" << std::endl;

    if ((_DisplayWindow= (void*)XOpenDisplay(0))==NULL)
    {
        std::cout << "Fail to connect to Display!" << std::endl;
    }


    std::cout << "Creating the Display Window" << std::endl;

    _XScreen = DefaultScreen(_DisplayWindow);


    int vidModeMajorVersion, vidModeMinorVersion;    

    XF86VidModeQueryVersion((Display*)_DisplayWindow, &vidModeMajorVersion, &vidModeMinorVersion);

    std::cout << "XF86VidModeExtension-Version " << vidModeMajorVersion << "." << vidModeMinorVersion << std::endl;


    std::cout << "Getting all available Video Modes" << std::endl;

    XF86VidModeModeInfo **modes;

    int nbmodes=0;        
    int bestmode=0;

    XF86VidModeGetAllModeLines((Display*)_DisplayWindow, _XScreen, &nbmodes, &modes);                

    /* save desktop-resolution before switching modes */
    _XDesktopMode = *modes[0];       

    std::cout << "Trying to set your requested Video Mode" << std::endl;

    for (int i=0; i < nbmodes; i++)
    {
        if ((modes[i]->hdisplay == xres) && (modes[i]->vdisplay == yres))
        {
            bestmode = i;
        }
    }


    XVisualInfo *xvisual=NULL;

    //try to set ColorDepth and double buffering
    //if requested mode fail, fall on smaller mode

    switch (bpp)
    {
    case 32:
        xvisual = glXChooseVisual((Display*)_DisplayWindow, _XScreen, attrListDbl32);

        if (xvisual == NULL)
            xvisual = glXChooseVisual((Display*)_DisplayWindow, _XScreen, attrListSgl32);

        if (xvisual!=NULL)
            break;

    case 24:
        xvisual = glXChooseVisual((Display*)_DisplayWindow, _XScreen, attrListDbl24);
        
        if (xvisual == NULL)
            xvisual = glXChooseVisual((Display*)_DisplayWindow, _XScreen, attrListSgl24);

        if (xvisual!=NULL)
            break;

    default://16bpp
        xvisual = glXChooseVisual((Display*)_DisplayWindow, _XScreen, attrListDbl16);

        if (xvisual == NULL)
            xvisual = glXChooseVisual((Display*)_DisplayWindow, _XScreen, attrListSgl16);

    }

    if (xvisual==NULL)
        std::cout << "Unable to set Color Depth!" << std::endl;



    std::cout << "Creating GLX Context" << std::endl;

    _XGLXContext = glXCreateContext((Display*)_DisplayWindow, xvisual, 0, GL_TRUE);        


    int glx_major_version, glx_minor_version;

    glXQueryVersion((Display*)_DisplayWindow, &glx_major_version, &glx_minor_version);

    std::cout << "GLX-Version " << glx_major_version << "." << glx_minor_version << std::endl;

    /* create a color map */

    Colormap cmap;

    cmap = XCreateColormap((Display*)_DisplayWindow, RootWindow((Display*)_DisplayWindow, xvisual->screen), xvisual->visual, AllocNone);

    _XWindowAttributes.colormap = cmap;

    _XWindowAttributes.border_pixel = 0;


    _XWindowAttributes.event_mask =  KeyPressMask | ButtonPressMask | StructureNotifyMask | KeyReleaseMask | ButtonReleaseMask | PointerMotionMask;

    if (fullscreen)
    {
        std::cout << "Switching to Requested FullScreen Video Resolution" << std::endl;

        XF86VidModeSwitchToMode((Display*)_DisplayWindow, _XScreen, modes[bestmode]);
        XF86VidModeSetViewPort((Display*)_DisplayWindow, _XScreen, 0, 0);

        //free the VideoModes Array
        XFree(modes);

        _XWindowAttributes.override_redirect = True;

        std::cout << "Creating the Window" << std::endl;

        _XWin = (long unsigned int)XCreateWindow((Display*)_DisplayWindow, RootWindow((Display*)_DisplayWindow, xvisual->screen),
                                                  0, 0, xres, yres, 0, xvisual->depth, InputOutput, xvisual->visual,
                                                  CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
                                                  &_XWindowAttributes);

        XWarpPointer((Display*)_DisplayWindow, None, _XWin, 0, 0, 0, 0, 0, 0);
        XMapRaised((Display*)_DisplayWindow, _XWin);
        XGrabKeyboard((Display*)_DisplayWindow, _XWin, True, GrabModeAsync, GrabModeAsync, CurrentTime);
        XGrabPointer((Display*)_DisplayWindow, _XWin, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, _XWin, None, CurrentTime);
    }
    else
    {   /* create a window in window mode*/

        std::cout << "Creating the Window" << std::endl;

        _XWin = XCreateWindow((Display*)_DisplayWindow, RootWindow((Display*)_DisplayWindow, xvisual->screen),
                               0, 0, xres, yres, 0, xvisual->depth, InputOutput, xvisual->visual,
                               CWBorderPixel | CWColormap | CWEventMask, &_XWindowAttributes);

        /* only set window title if in windowed mode */
        XSetStandardProperties((Display*)_DisplayWindow, _XWin, "Drone VideoOutput", "Drone VideoOutput", None, NULL, 0, NULL);
        XMapRaised((Display*)_DisplayWindow, _XWin);
    }       

    
    Window win_dummy;

    unsigned int border_dummy;        
    int x=0,y=0;
        
    XGetGeometry((Display*)_DisplayWindow, _XWin, &win_dummy, &x, &y, (unsigned int*)&xres, (unsigned int*)&yres, &border_dummy, (unsigned int*)&bpp);


    std::cout << "Depth = " << bpp << std::endl;

    if (glXIsDirect((Display*)_DisplayWindow, _XGLXContext))
        std::cout << "Direct Rendering Available" << std::endl;
    else
        std::cout << "Direct Rendering NOT Available" << std::endl;


    //enabling quit event   
    //Atom wmdelete;    

    //wmdelete = XInternAtom((Display*)_DisplayWindow, "WM_DELETE_WINDOW", True);
    //XSetWMProtocols((Display*)_DisplayWindow, (Window)_XWin, &wmdelete, 1);

    return 0;
}


int Gear_VideoOutput::initGL(int xres, int yres)
{   
    //it's important to init the context only there
    //because the context need to be created by the rendering thread
    //so we call initgl in prepaly that is called from the playthread of the engine 
    //each time we start playing    
    std::cout << "Making GLX context current" << std::endl;
    glXMakeCurrent((Display*)_DisplayWindow, _XWin, _XGLXContext);
    
    glEnable(GL_TEXTURE_2D);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST); 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_ALPHA_TEST);
    //glDisable(GL_CULL_FACE);     
    glEnable(GL_BLEND);
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    //glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, xres, yres);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
    glOrtho(0,xres, yres, 0, -99999, 99999);
    //gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();    
    swapBuffers();

    return 0;
}

void Gear_VideoOutput::logGLSpecs()
{

    std::cout << "Driver Info " << std::endl;
    std::cout << "vendor " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "render " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "version " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Support " << std::endl;


    int i=0;
    unsigned char* ext=(unsigned char*)glGetString(GL_EXTENSIONS);

    if (!ext)
        return;

    while (ext[i]!='\0')
    {
        if (ext[i]==' ')
        {
            std::cout << std::endl;
        }
        else
            std::cout << ext[i];

        i++;
    }

    int max_texture_size=0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    std::cout << "\nGL_MAX_TEXTURE_SIZE : " << max_texture_size << std::endl;
}

void Gear_VideoOutput::swapBuffers()
{
    glXWaitGL();
    glXSwapBuffers((Display*)_DisplayWindow, _XWin);    
}

void Gear_VideoOutput::destroyDisplayWindow(bool fullscreen)
{    
    if (_DisplayWindow == NULL)
        return;
    
    std::cout << "Destruction of the Display Window..." << std::endl;

    if (_XGLXContext)
    {
        if (!glXMakeCurrent((Display*)_DisplayWindow, None, NULL))
            std::cout << "Could not release drawing context" << std::endl;

        glXDestroyContext((Display*)_DisplayWindow, _XGLXContext);
        _XGLXContext = NULL;
    }

    //switch back to original desktop resolution if we were in fs 
    if (fullscreen)
    {
        XF86VidModeSwitchToMode((Display*)_DisplayWindow, _XScreen, & _XDesktopMode);
        XF86VidModeSetViewPort((Display*)_DisplayWindow, _XScreen, 0, 0);
    }

    std::cout << "Closing the Display" << std::endl;
    XFlush((Display*)_DisplayWindow);                              
}

