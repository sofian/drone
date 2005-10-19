/* Gear_TA_DataSource.cpp
 * Copyright (C) 2005 Jean-Sebastien Senecal
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
//inspired from Using libavformat and libavcodec by Martin Böhme (boehme@inb.uni-luebeckREMOVETHIS.de) 

#include <iostream>
#include "Gear_TA_DynamicRectangles.h"
#include "Engine.h"

#include "GearMaker.h"
#include "Random.h"

//#include "AlphaComposite.h"
#include "Utils.h"

#define MAX_RECTANGLES 5

void Rectangle::clipImage(const VideoRGBAType& imageIn)
{
  //  NOTICE("%p clip: %d %d", this, img.width(), img.height());
  RGBA *out = img.data();
  const RGBA *in = imageIn.data();
  for (int j=0, y=area.y0(); j<(int)img.height(); ++j, ++y)
  {
    if (0 <= y && y < imageIn.height()) // inside image
    {
      in = imageIn.row(y);
      for (int i=0, x=area.x0(); i<(int)img.width(); ++i, ++x, ++in)
      {
        if (0 <= x && x < imageIn.width()) // inside image
          *out++ = *in;
        else
          *out++ = CLEAR_RGBA;
      }
    }
    else
    {
      std::fill(out, out + img.width(), CLEAR_RGBA);
      out + img.width();
       //      std::fill(out, out += img.width(), CLEAR_RGBA);
    }
  }

}

// void Rectangle::recomputeAlpha()
// {
//   unsigned char alpha = CLAMP0255( (unsigned char) blend * 255 );
//   unsigned char *it = (unsigned char*)img.data();
//   for (size_t i=0; i<img.size(); ++i)
//   {
//     it[IDX_RGBA_A] = alpha;
//     it += SIZE_RGBA;
//   }
// }

void Rectangle::init(float gamma)
{
  _lut[0] = 0;
  float invGamma = 1.0 / gamma;
  for (int i=1; i<256; ++i)
    _lut[i] = CLAMP0255( ROUND(255.0f * exp( invGamma * log( float(i) / 255.0f ) ) ));
}

void Rectangle::applyGamma()
{
  //  NOTICE("%p gamma: %d %d", this, img.width(), img.height());
  unsigned char *data = (unsigned char*) img.data();

  // Apply gamma on img.
  for (int p=0; p<img.size(); ++p)
  {
    data[0] = _lut[data[0]];
    data[1] = _lut[data[1]];
    data[2] = _lut[data[2]];
    //data[3] = data[3];

    data += SIZE_RGBA;
  }
}

void Rectangle::run(VideoRGBAType& image) const
{
  //img.setIsAlphaPremultiplied(false);
  //img.premultiplyAlpha();

  unsigned char alpha = 255 - (unsigned char) (CLAMP( blend, 0.0f, 1.0f) * 255.0f);
  //  NOTICE("alpha: %d", (int)alpha);//unsigned char alpha = 127;
  unsigned char *imagePtr, *imgPtr;
  unsigned int tmp;
  // Copy everything.
  //memcpy(imageOut.data(), imageIn.data(), image.size() * sizeof(RGBA));

  // Copy the subwindow.
  for (int j=0, y=area.y0(); j<(int)area.height(); ++j, ++y)
  {
     if (0 <= y && y < image.height()) // inside image
     {
       for (int i=0, x=area.x0(); i<(int)area.width(); ++i, ++x)
       {
         if (0 <= x && x < image.width()) // inside image
         {
           imagePtr = (unsigned char*)&image(x,y);
           imgPtr = (unsigned char*)&img(i,j);
           imagePtr[0] = INT_BLEND(imagePtr[0], imgPtr[0], alpha, tmp);
           imagePtr[1] = INT_BLEND(imagePtr[1], imgPtr[1], alpha, tmp);
           imagePtr[2] = INT_BLEND(imagePtr[2], imgPtr[2], alpha, tmp);
         }
       }
     }
  }
  //  img.demultiplyAlpha();
}


extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_TA_DynamicRectangles(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "TA_DynamicRectangles";
    gearInfo.classification = GearClassifications::unclassified().instance();
    return gearInfo;
  }
}

Gear_TA_DynamicRectangles::Gear_TA_DynamicRectangles(Schema *schema, std::string uniqueName) : 
Gear(schema, "TA_DynamicRectangles", uniqueName)
{
  addPlug(_VIDEO_IN = new   PlugIn<VideoRGBAType>(this,"ImgIN",true)); 
//   addPlug(_INNOCENCE_IN = new PlugIn<ValueType>(this, "Ict", false));
//   addPlug(_CHANNEL_IN = new PlugIn<ValueType>(this, "Ch", false));
  addPlug(_WIDTH_IN = new PlugIn<ValueType>(this, "Width", false, new ValueType(768, 240, 1024)));
  addPlug(_HEIGHT_IN = new PlugIn<ValueType>(this, "Height", false, new ValueType(480, 180, 768)));
  
  addPlug(_GAMMA_IN = new PlugIn<ValueType>(this, "Gamma", false));

  addPlug(_DECAY_IN = new PlugIn<ValueType>(this, "Decay", false, new ValueType(0.001, 0, 1)));
  addPlug(_NEW_IN = new PlugIn<ValueType>(this, "New", false));

  addPlug(_VIDEO_OUT  = new PlugOut<VideoRGBAType>(this,"ImgOUT",true));

  _imageInBuffer = new VideoRGBAType();

  Random::manualSeed((int)'q');
//   _gamma = FLT_MIN;
//   computeGammaLookUpTable();
}

Gear_TA_DynamicRectangles::~Gear_TA_DynamicRectangles()
{
  _rectangles.clear();
  delete _imageInBuffer;
}

void Gear_TA_DynamicRectangles::runVideo()
{
  const VideoRGBAType *_imageIn = _VIDEO_IN->type();
  VideoRGBAType *_imageOut = _VIDEO_OUT->type();

  float decay = CLAMP(_DECAY_IN->type()->value(), 0.0f, 1.0f);
  
  int width = _WIDTH_IN->type()->intValue();
  int height = _HEIGHT_IN->type()->intValue();
  
  _imageOut->resize(_imageIn->width(), _imageIn->height());
  memcpy(_imageOut->data(), _imageIn->data(), _imageIn->size() * sizeof(RGBA));
  
  if (_NEW_IN->type()->boolValue())
    createRectangle(width, height, MAX(_GAMMA_IN->type()->value(), FLT_MIN));

  //  std::cout << "N rectangles: " << _rectangles.size() << std::endl;

  for (RectangleList::iterator it = _rectangles.begin(); it != _rectangles.end(); ++it)
  {
    // Compose image.
    it->clipImage(*_imageIn);
    it->applyGamma();
    //it->recomputeAlpha();
    it->run(*_imageOut);
    
    // Decay weight;
    it->blend -= decay;
    //NOTICE("Blend: %f, decay %f", it->blend, decay);
  }

  //_imageOut->demultiplyAlpha();
}

void Gear_TA_DynamicRectangles::createRectangle(size_t width, size_t height, float gamma)
{
  size_t max = MAX(width, height);
  Rectangle rect;
  rect.area.setOrigin((int)( Random::uniform() * width),
                      (int)( Random::uniform() * height));
  rect.area.resize((size_t)MAX( Random::uniform() * max, 32.0f),
                   (size_t)MAX( Random::uniform() * max, 32.0f));
  rect.img.resize(rect.area.width(), rect.area.height());
  rect.blend = 1.0f;
  rect.init(gamma);
  _rectangles.push_back(rect);
  if (_rectangles.size() > MAX_RECTANGLES)
    _rectangles.pop_front();
  NOTICE("n rectangles: %d.", _rectangles.size());
}
