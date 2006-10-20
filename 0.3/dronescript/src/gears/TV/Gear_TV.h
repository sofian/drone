/* Gear_TV.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
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

#ifndef GEAR_TV_INCLUDED
#define GEAR_TV_INCLUDED

#include "GearControl.h"
#include "VideoRGBAType.h"

class Gear_TV : public GearControl
{
public:

  Gear_TV(Schema *schema, std::string uniqueName);
  virtual ~Gear_TV();

  PlugIn<VideoRGBAType>* VIDEO_IN(){return _VIDEO_IN;};

  void runVideo();

protected:
  GearGui *createGearGui(QCanvas *canvas);
  Control* internalCreateControl(ControlPanel *){return NULL;}//TODO finish this

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;

  //local var
  VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 
  unsigned char *_data;
  unsigned char *_tempData;
  RGBA *_outData;
  int _accR,_accG, _accB;

  int _sizeY;
  int _sizeX;
  int _iterSizeY;
  int _iterSizeX;
  int _startX, _startY, _endX, _endY;

  int _blurSize;
  int _halfTVSize;

  RGBA _acc;



};

#endif
