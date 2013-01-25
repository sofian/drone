/* VideoOutput.h
 * Copyright (C) 2004 Mathieu Guindon
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

#ifndef GEAR_SHADER_TEST_INCLUDED
#define GEAR_SHADER_TEST_INCLUDED


#include <QtOpenGL>

#include "Gear.h"
#include "TextureType.h"

class Gear_ShaderTest : public Gear
{
public:

  Gear_ShaderTest(Schema *schema, std::string uniqueName);
  virtual ~Gear_ShaderTest();

  virtual void runVideo();
  virtual bool ready();

  
protected:
  virtual void internalInit();

private:
  void initializeShaderProgram();
  //void enumarateActiveUniforms();
  void enableGLStates();

  PlugIn<TextureType> *_TEXTURE_IN;
  PlugOut<TextureType> *_TEXTURE_OUT;
  
  QGLShaderProgram _shaderProgram;
  int _vertexAttr;
  int _texCoordAttr;
  int _textureUniform;
//  FrameBufferObject _fbo;
  QGLFramebufferObject *_fbo;
  
};


#endif

