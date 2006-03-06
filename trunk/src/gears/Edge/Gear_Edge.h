/* Gear_Edge.h
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

#ifndef GEAR_EDGE_INCLUDED
#define GEAR_EDGE_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"


class Gear_Edge : public Gear
{
public:

  Gear_Edge(Schema *schema, std::string uniqueName);
  virtual ~Gear_Edge();

  void runVideo();
  bool ready();

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_AMOUNT_IN;

  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 
  unsigned char *_data;
  unsigned char *_outData;

  int _sizeY;
  int _sizeX;    

  unsigned char *_iterUp;
  unsigned char *_iterCenter;
  unsigned char *_iterLeft;
  unsigned char *_iterRight;
  unsigned char *_iterDown;
  
  unsigned char *_iterOutData;

  //  RGBA _adjust[2];

//   double *_mmxImageInL0;
//   double *_mmxImageInL02;
//   double *_mmxImageInL1;
//   double *_mmxImageInL12;
//   double *_mmxImageInL2;
//   double *_mmxImageInL22;
//   double *_mmxImageOut;    
//   double *_mmxAdjust;



};

#endif
