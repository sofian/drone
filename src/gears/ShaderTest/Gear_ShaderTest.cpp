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

#define GL_GLEXT_PROTOTYPES

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

Gear_ShaderTest::Gear_ShaderTest(Schema *schema, std::string uniqueName) : Gear(schema, "ShaderTest", uniqueName), _fbo(NULL)
{
  addPlug(_TEXTURE_IN = new PlugIn<TextureType>(this, "TexIN", true));
  addPlug(_TEXTURE_OUT = new PlugOut<TextureType>(this, "TexOUT", true));
}

Gear_ShaderTest::~Gear_ShaderTest()
{
  if (_fbo)
  {
    delete _fbo;
    _fbo = NULL;
  }
}

void Gear_ShaderTest::internalInit()
{
  initializeShaderProgram();
  //enumarateActiveUniforms();
  
  if (_fbo)
  {
    delete _fbo;
    _fbo = NULL;
  }
}

bool Gear_ShaderTest::ready()
{
  return(_TEXTURE_IN->connected() && _TEXTURE_OUT->connected());
}

void Gear_ShaderTest::runVideo()
{
  const TextureType *inputTexture = _TEXTURE_IN->type();
  TextureType *outputTexture = _TEXTURE_OUT->type();

  enableGLStates();

  //create the fbo if not already done
  //recreate the fbo if input size changed
  if (_fbo==NULL)
  {
    _fbo = new QGLFramebufferObject(inputTexture->textureSizeX(), inputTexture->textureSizeY());
  }
  else if (_fbo->size().width() != inputTexture->textureSizeX() || _fbo->size().height() != inputTexture->textureSizeY())
  {
    delete _fbo;
    _fbo = new QGLFramebufferObject(inputTexture->textureSizeX(), inputTexture->textureSizeY());
  }
  
  //set the fbo texture name and size to the output texture
  outputTexture->setTextureName(_fbo->texture());
  outputTexture->setTextureSizeX(inputTexture->textureSizeX());
  outputTexture->setTextureSizeY(inputTexture->textureSizeY());
  
  _fbo->bind();
  
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
  
  _fbo->release();
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
  const char *vsrc1 =
  "attribute vec3 vertex;\n"
  "attribute vec2 texCoord;\n"
  "varying vec2 texc;\n"
  "void main(void)\n"
  "{\n"
  "    texc = texCoord;\n"
  "    gl_Position = vec4(vertex.x, vertex.y, vertex.z, 1.0);\n"
  "}\n";

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
  
  _shaderProgram.addShaderFromSourceCode(QGLShader::Vertex, vsrc1);
  _shaderProgram.addShaderFromSourceCode(QGLShader::Fragment, fsrc1);
  _shaderProgram.link();
  
  _vertexAttr = _shaderProgram.attributeLocation("vertex");
  _texCoordAttr = _shaderProgram.attributeLocation("texCoord");
  _textureUniform = _shaderProgram.uniformLocation("tex");
  
}

//void Gear_ShaderTest::enumarateActiveUniforms()
//{
//  GLint maxUniformLen;
//  GLint numUniforms;
//  char *uniformName;
//  GLint index;
//
//  glGetProgramiv(_shaderProgram.programId(), GL_ACTIVE_UNIFORMS, &numUniforms);
//  glGetProgramiv(_shaderProgram.programId(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);
//
//  uniformName = malloc(sizeof(char) * maxUniformLen);
//
//  std::cout << "--== Enumerating uniforms for program " << _shaderProgram.programId() << " ==--" << std::endl;
//
//  for(index = 0; index < numUniforms; index++)
//  {
//    GLint size;
//    GLenum type;
//    GLint location;
//
//    // Get the Uniform Info
//    glGetActiveUniform(_shaderProgram.programId(), index, maxUniformLen, NULL, &size, &type, uniformName);
//    std::cout << "Uniform name: " << uniformName << std::endl;
//
//    // Get the uniform location
//    location = glGetUniformLocation(_shaderProgram.programId(), uniformName);
//    std::cout << "Uniform location: " << location << std::endl;
//
//    std::cout << "Uniform type: ";
//    switch(type)
//    {
//      case GL_FLOAT:
//        std::cout << "GL_FLOAT" << std::endl;
//        break;
//      case GL_FLOAT_VEC2:
//        std::cout << "GL_FLOAT_VEC2" << std::endl;
//        break;
//      case GL_FLOAT_VEC3:
//        std::cout << "GL_FLOAT_VEC3" << std::endl;
//        break;
//      case GL_FLOAT_VEC4:
//        std::cout << "GL_FLOAT_VEC4" << std::endl;
//        break;
//      case GL_INT:
//        std::cout << "GL_INT" << std::endl;
//        break;
//      case GL_SAMPLER_2D:
//        std::cout << "GL_SAMPLER_2D" << std::endl;
//        break;
//      default:
//        std::cout << "Unsupported type" << std::endl;
//        break;
//    }
//  }
//}
