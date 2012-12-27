/* Gear_ShaderTest.cpp
 * Copyright (C) 2012 Mathieu Guindon
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Gear_ShaderTest.h"                       
#include "Engine.h"

#if defined(Q_OS_MACX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#endif

#include <iostream>

#include "GearMaker.h"

GLfloat afVertices[] = {
  -1,-1,0,
  -1,1,0,
  1,-1,0,
  1,1,0,
};

GLfloat afTexCoord[] = {
  0,0,
  0,1,
  1,0,
  1,1
};


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_ShaderTest(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "ShaderTest";
  gearInfo.classification = GearClassifications::video().blur().instance();
  return gearInfo;
}
}

Gear_ShaderTest::Gear_ShaderTest(Schema *schema, std::string uniqueName) : Gear(schema, "ShaderTest", uniqueName)
{
  addPlug(_TEXTURE_IN = new PlugIn<TextureType>(this, "TexIN", true));
  addPlug(_TEXTURE_OUT = new PlugOut<TextureType>(this, "TexOUT", true));
}

Gear_ShaderTest::~Gear_ShaderTest()
{

}

void Gear_ShaderTest::internalInit()
{
  initializeShaderProgram();
  _fbo.create();
}

bool Gear_ShaderTest::ready()
{
  return(_TEXTURE_IN->connected() && _TEXTURE_OUT->connected());
}

void Gear_ShaderTest::runVideo()
{
  TextureType *inputTexture = _TEXTURE_IN->type();
  TextureType *outputTexture = _TEXTURE_OUT->type();

  enableGLStates();
  
  outputTexture->createWithSize(inputTexture->textureSizeX(), inputTexture->textureSizeY());
  
  _fbo.attachTexture(outputTexture->textureName());
  _fbo.bind();

  if (!_fbo.isReady())
  {
    std::cerr << "fbo not ready!" << std::endl;
  }
  
  glViewport(0, 0, inputTexture->textureSizeX(), inputTexture->textureSizeY());
  
  _shaderProgram.bind();

  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);  
  glBindTexture(GL_TEXTURE_2D, inputTexture->textureName());
  
  _shaderProgram.setUniformValue(_textureUniform, 0);
  _shaderProgram.enableAttributeArray(_vertexAttr);
  _shaderProgram.enableAttributeArray(_texCoordAttr);
  _shaderProgram.setAttributeArray(_vertexAttr, afVertices, 3);
  _shaderProgram.setAttributeArray(_texCoordAttr, afTexCoord, 2);

  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  _fbo.unBind();
}

void Gear_ShaderTest::enableGLStates()
{
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DITHER);
  glDisable(GL_DEPTH_TEST);
}

void Gear_ShaderTest::initializeShaderProgram()
{
  QGLShader *vshader1 = new QGLShader(QGLShader::Vertex, this);
  const char *vsrc1 =
  "attribute vec3 vertex;\n"
  "attribute vec2 texCoord;\n"
  "varying vec2 texc;\n"
  "void main(void)\n"
  "{\n"
  "    texc = texCoord;\n"
  "    gl_Position = vec4(vertex.x, vertex.y, vertex.z, 1.0);\n"
  "}\n";
  vshader1->compileSourceCode(vsrc1);
  
  QGLShader *fshader1 = new QGLShader(QGLShader::Fragment, this);
  const char *fsrc1 =
  "varying vec2 texc;\n"
  "uniform sampler2D tex;\n"
  "const float blurSize = 1.0/512.0;\n"
  "void main(void)\n"
  "{\n"
  "vec4 sum = vec4(0.0);\n"
  "// blur in y (vertical)\n"
  "// take nine samples, with the distance blurSize between them\n"
  "sum += texture2D(tex, vec2(texc.x - 4.0*blurSize, texc.y)) * 0.05;\n"
  "sum += texture2D(tex, vec2(texc.x - 3.0*blurSize, texc.y)) * 0.09;\n"
  "sum += texture2D(tex, vec2(texc.x - 2.0*blurSize, texc.y)) * 0.12;\n"
  "sum += texture2D(tex, vec2(texc.x - blurSize, texc.y)) * 0.15;\n"
  "sum += texture2D(tex, vec2(texc.x, texc.y)) * 0.16;\n"
  "sum += texture2D(tex, vec2(texc.x + blurSize, texc.y)) * 0.15;\n"
  "sum += texture2D(tex, vec2(texc.x + 2.0*blurSize, texc.y)) * 0.12;\n"
  "sum += texture2D(tex, vec2(texc.x + 3.0*blurSize, texc.y)) * 0.09;\n"
  "sum += texture2D(tex, vec2(texc.x + 4.0*blurSize, texc.y)) * 0.05;\n"
  "gl_FragColor = sum;}\n";
  
  fshader1->compileSourceCode(fsrc1);
  
  _shaderProgram.addShader(vshader1);
  _shaderProgram.addShader(fshader1);
  //todo mg: delete vshader and fshader
  _shaderProgram.link();
  
  _vertexAttr = _shaderProgram.attributeLocation("vertex");
  _texCoordAttr = _shaderProgram.attributeLocation("texCoord");
  _textureUniform = _shaderProgram.uniformLocation("tex");
  
}
