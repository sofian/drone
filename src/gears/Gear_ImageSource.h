/* Gear_ImageSource.h
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

#ifndef GEAR_IMAGESOURCE_INCLUDED
#define GEAR_IMAGESOURCE_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"
//#include "ColorSpace.h"

#include <qimage.h>

class Gear_ImageSource : public Gear
{
public:
  static const std::string SETTING_FILENAME;

  Gear_ImageSource(Schema *schema, std::string uniqueName);
  virtual ~Gear_ImageSource();

  void runVideo();

  bool ready();

protected:
  void onUpdateSettings();

private:
  PlugOut<VideoRGBAType> *_VIDEO_OUT;

  Array2D<RGBA> _imageBuffer;

  int _sizeX, _sizeY;
};

#endif
