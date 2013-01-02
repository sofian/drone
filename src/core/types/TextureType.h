//
//  TextureType.h
//  drone
//
//  Created by Mathieu Guindon on 2012-12-18.
/*
* Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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


#ifndef drone_TextureType_h
#define drone_TextureType_h

#include "AbstractType.h"
#include "VideoRGBAType.h"

#include <string>

class TextureType : public AbstractType
{
public:
  TextureType();
  
  virtual ~TextureType();

  TYPE_BASE_METHODS(TextureType, (255, 255, 64))
  
  void setTextureName(unsigned int value) { _textureName = value; }
  unsigned int textureName() const { return _textureName; }
  
  unsigned int createWithSize(int textureSizeX, int textureSizeY, bool forceCreation = false);
  unsigned int updateWithVideoRGBA(const VideoRGBAType &image);
  unsigned int textureSizeX() const {return _textureSizeX;}
  unsigned int textureSizeY() const {return _textureSizeY;}

  void setTextureSizeX(unsigned int value) {_textureSizeX = value;}
  void setTextureSizeY(unsigned int value) {_textureSizeY = value;}
  
private:
	unsigned int _textureName;
  unsigned int _textureSizeX;
  unsigned int _textureSizeY;
  
};

#endif
