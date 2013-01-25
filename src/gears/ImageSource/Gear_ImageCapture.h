/* Gear_ImageCapture.h
 * Copyright (C) 2004--2005 Jean-Sebastien Senecal
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

#ifndef GEAR_IMAGECAPTURE_INCLUDED
#define GEAR_IMAGECAPTURE_INCLUDED

#include "Gear.h"
#include "VideoRGBAType.h"
#include "ValueType.h"

#include <qimage.h>

class Gear_ImageCapture : public Gear
{
public:
  static const QString SETTING_FILENAME;
  static const QString FORMAT_EXTENSION;
  static const QString DEFAULT_FILENAME;

  Gear_ImageCapture(Schema *schema, QString name);
  virtual ~Gear_ImageCapture();

  void runVideo();

private:
  // Inputs.
  PlugIn<ValueType> *_CAPTURE_IN;
  
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  
  // Functions.
  void saveImage(const QString& filename, const VideoRGBAType *image);

};

#endif
