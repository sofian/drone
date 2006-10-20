/* Gear_Invert.h
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

#ifndef GEAR_AMUSEOEIL_INCLUDED
#define GEAR_AMUSEOEIL_INCLUDED

#include "Gear.h"
#include "VideoRGBAType.h"

struct stripe_t
{
  
/* a stripe is defined by 

0 y
1 y1
2 y2
3 h
4 s
5 v
6 a

*/

  float current[7];
  float last[7];
  float target[7];
  float initial[7];
  float v[7];
  float a[7];
  // mode : 0: arriving, 1: alive, 2: dying 3: dead
  int mode;
  int transisteps;
  int transicount;
  float hickup;
};

class Gear_AmuseOeil : public Gear
{
public:

  Gear_AmuseOeil(Schema *schema, std::string name);
  virtual ~Gear_AmuseOeil();

  void runVideo();  
  bool ready();


private:
  int aliveStripes();
  void killStripe();

  void addStripe();
  void updateStripes();

  //number of stripes
  PlugIn<ValueType>* _P1_IN;  
  
  // friction
  PlugIn<ValueType>* _FRIC_IN;  
  // p3
  PlugIn<ValueType>* _ACCEL_IN;  
  // change smoothness
  PlugIn<ValueType>* _P4_IN;  

  // alpha avg 
  PlugIn<ValueType>* _HAVG_IN;  
  // h avg
  PlugIn<ValueType>* _SAVG_IN;  
  // s avg
  PlugIn<ValueType>* _VAVG_IN;  
  // v avg
  PlugIn<ValueType>* _AAVG_IN;  


  // alpha span
  PlugIn<ValueType>* _HSPAN_IN;
  // h span
  PlugIn<ValueType>* _SSPAN_IN;
  // s span
  PlugIn<ValueType>* _VSPAN_IN;
  // v span
  PlugIn<ValueType>* _ASPAN_IN;
  
  // speed
  PlugIn<ValueType>* _SPEED_IN;  

  // stripes width scaling
  PlugIn<ValueType>* _SCALE_IN;  

  // frequency of perturbations
  PlugIn<ValueType>* _PERTURB_IN;  

  // instant n pertubation 
  PlugIn<ValueType>* _IPERTURB_IN;  
  

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 
  const unsigned int *_data;
  unsigned int *_outData;
  
  std::vector<stripe_t> _stripes;

  float _curscale;
//  int _iterSizeY;
//  int _iterSizeX;    

//  unsigned char *_imageIn;
//  unsigned char *_imageOut;

};

#endif
