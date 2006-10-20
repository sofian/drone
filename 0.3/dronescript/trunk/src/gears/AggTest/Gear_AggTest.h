/* Gear_Saturation.h
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

#ifndef GEAR_AGGTEST_INCLUDED
#define GEAR_AGGTEST_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"


class Gear_AggTest : public Gear
{
public:

  Gear_AggTest(Schema *schema, std::string uniqueName);
  virtual ~Gear_AggTest();

  void runVideo();  bool ready();
  
protected:
  void internalInit();

private:

  void newPoint(int sizex, int sizey, int pt_source=-1);
  void animatePoints(int sizex, int sizey,float,float,float,float,float,float);

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_AMOUNT_IN;
  PlugIn<ValueType> *_BOXX1_IN;
  PlugIn<ValueType> *_BOXX2_IN;
  PlugIn<ValueType> *_BOXY1_IN;
  PlugIn<ValueType> *_BOXY2_IN;
  PlugIn<ValueType> *_FRIC_IN;
  PlugIn<ValueType> *_REPUL_IN;

  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 

  int _size;

  int _r;
  int _g;
  int _b;

  const unsigned char *_imageIn;
  unsigned char *_imageOut;

};

#endif
