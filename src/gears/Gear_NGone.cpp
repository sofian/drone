#include "Gear_NGone.h"
#include "Engine.h"

#include "GearMaker.h"

#include <GL/gl.h>              
#include <GL/glu.h>
#include <math.h>
#include <iostream>

Register_Gear(MAKERGear_NGone, Gear_NGone, "NGone")

Gear_NGone::Gear_NGone(Engine *engine, std::string name) : Gear(engine, "NGone", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "Img"));
  addPlug(_SIGNAL_X_IN = new PlugIn<SignalType>(this, "X", new SignalType(0.0f)));
  addPlug(_SIGNAL_Y_IN = new PlugIn<SignalType>(this, "Y", new SignalType(0.0f)));
}

Gear_NGone::~Gear_NGone()
{

}

bool Gear_NGone::ready()
{
  return(_VIDEO_IN->connected());
}

void Gear_NGone::runVideo()
{
  _image = _VIDEO_IN->type();
  _sizeX = 512;
  _sizeY = 512;
  _texSizeX = _image->width() / _sizeX;
  _texSizeY = _image->height() / _sizeY;

  _signalBufferX = _SIGNAL_X_IN->type()->data();
  _signalBufferY = _SIGNAL_Y_IN->type()->data();

  int subDivisions=64;

  float xSinOffset=0.0f;
  float ySinOffset=0.0f;

//    glBlendFunc(GL_SRC_ALPHA,GL_ONE);    

  float xSpacing = _sizeX / subDivisions;
  float ySpacing = _sizeY / subDivisions;

  int signalOffset = Engine::signalInfo().blockSize() / subDivisions;

  float xTexSpacing = _texSizeX / subDivisions;
  float yTexSpacing = _texSizeY / subDivisions;

//   glBindTexture(GL_TEXTURE_2D, _image->toTexture());

  glEnable(GL_TEXTURE_2D);

  //glPolygonMode( GL_FRONT, GL_LINE ); 
  //glPolygonMode( GL_BACK, GL_LINE );  

  static float st=0.0f;
  st+=0.0001f;
  int t=0;
  for (float y=0,yTex=0;y<_sizeY;y+=ySpacing,yTex+=yTexSpacing)
  {
    glBegin(GL_QUAD_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    t=0;
    for (float x=0,xTex=0;x<_sizeX;x+=xSpacing,xTex+=xTexSpacing,t+=signalOffset)
    {
      //ySinOffset = sin((st+t) * freq * 6.28318f) * amp;
      xSinOffset = _signalBufferX[t];
      ySinOffset = _signalBufferY[t];

      glTexCoord2f(xTex, yTex);
      glVertex2f(x + xSinOffset, y+ySinOffset);

      glTexCoord2f(xTex, yTex+yTexSpacing);
      glVertex2f(x + xSinOffset, y+ySpacing+ySinOffset);
    }
    glEnd();
  }


/*       //glPopMatrix();                          */
/*     glBegin(GL_QUADS);                          */
/*         glColor3f(1.0f, 1.0f, 1.0f);            */
/*             glTexCoord2f(0.0f, 0.0f);           */
/*             glVertex2f(0.0f, 0.0f);             */
/*                                                 */
/*             glTexCoord2f(_texSizeX, 0.0f);      */
/*             glVertex2f(_sizeX, 0.0f);           */
/*                                                 */
/*             glTexCoord2f(_texSizeX, _texSizeY); */
/*             glVertex2f(_sizeX, _sizeY);         */
/*                                                 */
/*             glTexCoord2f(0.0f, _texSizeY);      */
/*             glVertex2f(0.0f, _sizeY);           */
/*                                                 */
/*                                                 */
/*     glEnd();                                    */

}



