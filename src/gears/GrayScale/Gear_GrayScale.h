/* Gear_GrayScale.h
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

#ifndef GEAR_GRAYSCALE_INCLUDED
#define GEAR_GRAYSCALE_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "VideoChannelType.h"

class Gear_GrayScale : public Gear
{
public:

  Gear_GrayScale(Schema *schema, QString uniqueName);
  virtual ~Gear_GrayScale();

  void runVideo();

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugOut<VideoChannelType> *_CHANNEL_OUT;
  
  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage;
  VideoChannelType *_outChannel; 

//   //  int _size;

//   unsigned int *_imageOut;
//   unsigned char *_channelOut;
//   unsigned char *_imageIn;



};

#endif
