#include "VideoOutputGl.h"
#include "VideoOutputMaker.h"

#include <iostream>
#include "Canvas.h"

Register_VideoOutput(MAKERVideoOutputGL, VideoOutputGl, "Gl")

/* static int attrListSgl16[] = {GLX_RGBA, GLX_RED_SIZE, 5,   */
/*     GLX_GREEN_SIZE, 5,                                     */
/*     GLX_BLUE_SIZE, 5,                                      */
/*     GLX_DEPTH_SIZE, 16,                                    */
/*     None};                                                 */
/*                                                            */
/* static int attrListDbl16[] = { GLX_RGBA, GLX_DOUBLEBUFFER, */
/*     GLX_RED_SIZE, 5,                                       */
/*     GLX_GREEN_SIZE, 5,                                     */
/*     GLX_BLUE_SIZE, 5,                                      */
/*     GLX_DEPTH_SIZE, 16,                                    */
/*     None};                                                 */
/*                                                            */
/* static int attrListSgl24[] = {GLX_RGBA, GLX_RED_SIZE, 6,   */
/*     GLX_GREEN_SIZE, 6,                                     */
/*     GLX_BLUE_SIZE, 6,                                      */
/*     GLX_DEPTH_SIZE, 24,                                    */
/*     None};                                                 */
/*                                                            */
/* static int attrListDbl24[] = { GLX_RGBA, GLX_DOUBLEBUFFER, */
/*     GLX_RED_SIZE, 6,                                       */
/*     GLX_GREEN_SIZE, 6,                                     */
/*     GLX_BLUE_SIZE, 6,                                      */
/*     GLX_DEPTH_SIZE, 24,                                    */
/*     None};                                                 */
/*                                                            */
/* static int attrListSgl32[] = {GLX_RGBA, GLX_RED_SIZE, 8,   */
/*     GLX_GREEN_SIZE, 8,                                     */
/*     GLX_BLUE_SIZE, 8,                                      */
/*     GLX_DEPTH_SIZE, 32,                                    */
/*     None};                                                 */
/*                                                            */
/* static int attrListDbl32[] = { GLX_RGBA, GLX_DOUBLEBUFFER, */
/*     GLX_RED_SIZE, 8,                                       */
/*     GLX_GREEN_SIZE, 8,                                     */
/*     GLX_BLUE_SIZE, 8,                                      */
/*     GLX_DEPTH_SIZE, 32,                                    */
/*     None};                                                 */


VideoOutputGl::VideoOutputGl() :
    VideoOutputX11Base(),    
    _frameSizeX(0),
    _frameSizeY(0),
    _frameSize(0),
    _glInitialized(false)
{    
}

VideoOutputGl::~VideoOutputGl()
{
    destroy();
}

void VideoOutputGl::destroy()
{
    destroyXWindow();    

}

void VideoOutputGl::fullscreen(bool fs)
{
    togglefullscreen(fs, _xRes, _yRes);
}

void VideoOutputGl::render(Canvas &canvas)
{    
    processX11Events();

    //it's important to init the context only there
    //because the context need to be created by the rendering thread
    //so we call initgl in preplay that is called from the playthread of the engine 
    //each time we start playing    
    if (!_glInitialized)
    {
        initGl(_xRes, _yRes);
        _glInitialized=true;
    }
        
    _frameSizeX = _xRes;
    _frameSizeY = _yRes;
    _texSizeX = (float)canvas.sizeX() / (float)_frameSizeX;
    _texSizeY = (float)canvas.sizeY() / (float)_frameSizeY;

    glBindTexture(GL_TEXTURE_2D, canvas.toTexture());
    glEnable(GL_TEXTURE_2D);        

    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);

        glTexCoord2f(_texSizeX, 0.0f);
        glVertex2f(_frameSizeX, 0.0f);

        glTexCoord2f(_texSizeX, _texSizeY);
        glVertex2f(_frameSizeX, _frameSizeY);

        glTexCoord2f(0.0f, _texSizeY);
        glVertex2f(0.0f, _frameSizeY);

    glEnd();

    glXWaitGL();
    glXSwapBuffers((Display*)_display, _window);        
}

bool VideoOutputGl::init(int xRes, int yRes, int bpp, bool fullscreen)
{            
    std::cout << "--==|| Xv output initialization ||==--" << std::endl;
    
    _glInitialized=false;
    _xRes = xRes;
    _yRes = yRes;

    if (!openXDisplay())
        return false;
        
    if (!createXWindow(xRes, yRes))
        return false;

    if (!createGLXContext())
        return false;

    
    return true;
}

bool VideoOutputGl::createGLXContext()
{
    std::cout << "creating GLX Context...";

    _XGLXContext = glXCreateContext((Display*)_display, &_visualInfo, 0, GL_TRUE);        
    
    int glxMajorVersion, glxMinorVersion;

    if (glXQueryVersion((Display*)_display, &glxMajorVersion, &glxMinorVersion) == False)
    {
        std::cout << "FAIL! glx not available" << std::endl;
        return false;
    }

    if (!glXIsDirect((Display*)_display, _XGLXContext))
    {
        std::cout << "FAIL! direct rendering not available" << std::endl;
        return false;
    }   
     
    std::cout << "GLX-Version " << glxMajorVersion << "." << glxMinorVersion << std::endl;

    return true;

}

int VideoOutputGl::initGl(int xRes, int yRes)
{   
    std::cout << "Making GLX context current" << std::endl;
    glXMakeCurrent((Display*)_display, _window, _XGLXContext);
    
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
    glViewport(0, 0, xRes, yRes);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
    glOrtho(0,xRes, yRes, 0, -99999, 99999);
    //gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();    
    
    return 0;
}

void VideoOutputGl::onResize(int sizeX, int sizeY)
{
    glViewport(0, 0, sizeX, sizeY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
    glOrtho(0, sizeX, sizeY, 0, -99999, 99999);
    //gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();    
}
