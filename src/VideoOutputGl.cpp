#include "VideoOutputGl.h"
#include "VideoOutputMaker.h"

#include <iostream>
#include "Canvas.h"

using namespace X11;

Register_VideoOutput(MAKERVideoOutputGL, VideoOutputGl, "Gl")

VideoOutputGl::VideoOutputGl() :
VideoOutputX11Base(),    
_XGLXContext(NULL),
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

  destroyGLXContext();
}

void VideoOutputGl::fullscreen(bool fs)
{
  togglefullscreen(fs, _xRes, _yRes);
}

void VideoOutputGl::render(MatrixType<RGBA> &image)
{    
  processX11Events();

  if (_frameSizeX!=image.width() || _frameSizeY!=image.height())
  {
    _frameSizeX = image.width();
    _frameSizeY = image.height();

    if (!_glInitialized)
    {
      //it's important to init the context only there
      //because the context need to be created by the rendering thread
      //so we call initgl in render that well be called from the playthread of the engine 
      //each time we start playing                
      initGl(_frameSizeX, _frameSizeY);
      _glInitialized=true;
    } else
    {
      resizeWindow(_frameSizeX, _frameSizeY);
      resizeGl(_frameSizeX, _frameSizeY);
    }        
  }
/*                                                                  */
/*   _texSizeX = (float)_frameSizeX / (float)canvas.textureSizeX(); */
/*   _texSizeY = (float)_frameSizeY / (float)canvas.textureSizeY(); */
/*                                                                  */
/*   glBindTexture(GL_TEXTURE_2D, canvas.toTexture(_bpp));          */
/*   glEnable(GL_TEXTURE_2D);                                       */
/*                                                                  */
/*   glBegin(GL_QUADS);                                             */
/*   glColor3f(1.0f, 1.0f, 1.0f);                                   */
/*   glTexCoord2f(0.0f, 0.0f);                                      */
/*   glVertex2f(0.0f, 0.0f);                                        */
/*                                                                  */
/*   glTexCoord2f(_texSizeX, 0.0f);                                 */
/*   glVertex2f(_xRes, 0.0f);                                       */
/*                                                                  */
/*   glTexCoord2f(_texSizeX, _texSizeY);                            */
/*   glVertex2f(_xRes, _yRes);                                      */
/*                                                                  */
/*   glTexCoord2f(0.0f, _texSizeY);                                 */
/*   glVertex2f(0.0f, _yRes);                                       */
/*                                                                  */
/*   glEnd();                                                       */
/*                                                                  */
/*   glXWaitGL();                                                   */
/*   glXSwapBuffers((Display*)_display, _window);                   */
}

bool VideoOutputGl::init(int xRes, int yRes, bool fullscreen)
{            
  std::cout << "--==|| Gl output initialization ||==--" << std::endl;

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

void VideoOutputGl::destroyGLXContext()
{
  std::cout << "destroying GLX context...";

  if (_XGLXContext)
  {
    if (!glXMakeCurrent((Display*)_display, None, NULL))
      std::cout << "could not release GLX context";

    glXDestroyContext((Display*)_display, _XGLXContext);
    _XGLXContext = NULL;
  }

  std::cout << "done" << std::endl;
}

int VideoOutputGl::initGl(int xRes, int yRes)
{       
  glXMakeCurrent((Display*)_display, _window, _XGLXContext);

  glEnable(GL_TEXTURE_2D);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST); 
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);         
  glDisable(GL_DITHER);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  resizeWindow(xRes, yRes);
  resizeGl(xRes, yRes);
  return 0;
}

void VideoOutputGl::onResize(int sizeX, int sizeY)
{
  resizeGl(sizeX, sizeY);
}

void VideoOutputGl::resizeGl(int sizeX, int sizeY)
{
  glViewport(0, 0, sizeX, sizeY);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, sizeX, sizeY, 0, -99999, 99999);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

