/* Gear_VideoTexture.h
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#ifndef GEAR_VIDEOTEXTURE_INCLUDED
#define GEAR_VIDEOTEXTURE_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
#include "ValueType.h"

#include <libmpeg3.h>
#include <pthread.h>

class Gear_VideoTexture : public Gear
{
public:
  static const std::string SETTING_FILENAME;
  static const std::string SETTING_NFRAMES;
  static const std::string SETTING_TIMEWINDOWLENGTH;

  Gear_VideoTexture(Schema *schema, std::string uniqueName);
  virtual ~Gear_VideoTexture();

  void runVideo();

  bool ready();

protected:
  void onUpdateSettings();

private:

  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  //PlugIn<ValueType> *_SIGMA_IN;

  mpeg3_t* _file;

  //locals
  
  RGBA *_frame[1024];
  
  int64_t _bytes;

  pthread_mutex_t *_mutex;
  
  Array2D<double> _distances;
  Array2D<double> _smoothedDistances;
  Array2D<double> _logCumProbs;
  Array<double> _minDistances;
  
  Array< Array2D<RGBA> > _sequences;
  
  int _sizeX, _sizeY, _size;
  int _nFrames;
  int _currentFrame;

  static double L2(const unsigned char *src1, const unsigned char *src2, size_t n);
};

#endif
