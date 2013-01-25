/* Gear_VideoOffset.h
 * Copyright (C) 2004 Gephex crew
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

#ifndef GEAR_VIDEOOFFSET_INCLUDED
#define GEAR_VIDEOOFFSET_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"

class Gear_VideoOffset : public Gear
{
public:

  Gear_VideoOffset();
  virtual ~Gear_VideoOffset();

  void runVideo();  

private:


  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  PlugIn<ValueType>* _XOFF_IN;  
  PlugIn<ValueType>* _YOFF_IN;  


  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 
  const unsigned int *_data;
  unsigned int *_outData;

//  int _iterSizeY;
//  int _iterSizeX;    

//  unsigned char *_imageIn;
//  unsigned char *_imageOut;

};

#endif
