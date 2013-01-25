/* Gear_ImageCapture.cpp
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

#include <iostream>
#include <sstream>
#include <stdio.h>

#include "Gear_ImageCapture.h"
#include "Engine.h"


#include "DroneMath.h"

#include <qfile.h>


const QString Gear_ImageCapture::SETTING_FILENAME = "Filename";
const QString Gear_ImageCapture::FORMAT_EXTENSION = "png";
const QString Gear_ImageCapture::DEFAULT_FILENAME = "droneImgCapture";

extern "C" {
Gear* makeGear()
{
  return new Gear_ImageCapture();
}


}

Gear_ImageCapture::Gear_ImageCapture() : 
  Gear("ImageCapture")  
{
  // Inputs.
  addPlug(_CAPTURE_IN = new PlugIn<ValueType>(this, "Go", false, new ValueType(0, 0, 0)));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIn", true));
  

  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr(DEFAULT_FILENAME);
}

Gear_ImageCapture::~Gear_ImageCapture()
{
}

void Gear_ImageCapture::saveImage(const QString& filename, const VideoRGBAType *image)
{  
  QImage img(QSize(image->width(), image->height()), 32);
  
  unsigned char *data = (unsigned char*)image->data();
  for (int y = 0; y < img.height(); ++y)
    for (int x = 0; x < img.width(); ++x)
    {            
      img.setPixel(x, y, qRgba(*data, *(data+1), *(data+2), *(data+3)));      
      data+=4;
    }

  
  QString baseFilename = filename;
  QString filenameWithExtension;  
  //empty filename
  if (baseFilename.size()<=0)  
    baseFilename = DEFAULT_FILENAME;
   
  //remove extension from basefilename if already there
  if (baseFilename.endsWith(FORMAT_EXTENSION))
    baseFilename.remove(baseFilename.size()-FORMAT_EXTENSION.size(), baseFilename.size()-1);

  //already exist? find unique name
  int counter=1;
  
  std::ostringstream oss;
  oss << baseFilename.toStdString() << "." << FORMAT_EXTENSION.toStdString();
  while(QFile::exists(QString::fromStdString(oss.str())))
  {    
    oss.seekp(0);
    oss.str("");
    oss << baseFilename.toStdString() << counter << "." << FORMAT_EXTENSION.toStdString();
    counter++;
  }
    
  //save at best quality
  QString extensionUpperCase = FORMAT_EXTENSION.toUpper();
  img.save(oss.str().c_str(), extensionUpperCase.toAscii(), 100);
}

void Gear_ImageCapture::runVideo()
{
  if (_CAPTURE_IN->type()->value()!=0.0f)
  {
    saveImage(_settings.get(SETTING_FILENAME)->valueStr(), _VIDEO_IN->type());
  }
}



