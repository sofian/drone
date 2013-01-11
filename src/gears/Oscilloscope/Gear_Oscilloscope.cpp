/* Gear_Oscilloscope.cpp
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

#include "Gear_Oscilloscope.h"
#include "Engine.h"
#include <iostream>
#include "GearMaker.h"
#include "CircularBuffer.h"

extern "C" {
Gear* makeGear(Schema *schema, QString uniqueName)
{
	return new Gear_Oscilloscope(schema, uniqueName);
}

GearInfo getGearInfo()
{
	GearInfo gearInfo;
	gearInfo.name = "Oscilloscope";
	gearInfo.classification = GearClassifications::signal().visualization().instance();
	return gearInfo;
}
}

Gear_Oscilloscope::Gear_Oscilloscope(Schema *schema, QString uniqueName) : Gear(schema, "Oscilloscope", uniqueName)
{
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "In", true));
  addPlug(_ZOOM_X = new PlugIn<ValueType>(this, "ZoomX", false, new ValueType(44100,512,192400)));
  addPlug(_ZOOM_Y = new PlugIn<ValueType>(this, "ZoomY", false, new ValueType(1,0,2)));
  addPlug(_SIZE_X = new PlugIn<ValueType>(this, "SizeX", false, new ValueType(512,32,768)));
  addPlug(_SIZE_Y = new PlugIn<ValueType>(this, "SizeY", false, new ValueType(150,32,768)));

  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "Out", true));

  circbuf = new CircularBuffer<Signal_T>(0.0f);
}

Gear_Oscilloscope::~Gear_Oscilloscope()
{
  delete circbuf;
}

void Gear_Oscilloscope::runAudio()
{
  const SignalType *buffer = _AUDIO_IN->type();
  circbuf->resize(_AUDIO_IN->type()->size(), 150000 / _AUDIO_IN->type()->size());
  circbuf->append(buffer->data());
}

void Gear_Oscilloscope::runVideo()
{
  int sizey = CLAMP((int)_SIZE_Y->type()->value(),32,512);
  int sizex = CLAMP((int)_SIZE_X->type()->value(),32,1440);
  int midy = sizey>>1;
  int midym1 = midy-1;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(sizex,sizey);
  _outData = _outImage->data();

  float zoomy = _ZOOM_Y->type()->value();
  int zoomx = (int)_ZOOM_X->type()->value();
  int signal_blocksize = Engine::signalInfo().blockSize();

  // ? debug circularbuffer please .. crash when using 192400.. can't get total buffer ?
  zoomx=CLAMP(zoomx,signal_blocksize,191399);

  _outImage->fill(BLACK_RGBA);

  float samples_per_pixels = zoomx / (float)sizex;
  float samples_used=0, avg=0,absavg=0;
  int i=0,i2=0;

//  std::cerr<<"spp : "<<samples_per_pixels<< " zoomx :"<<zoomx<<"\n";
  
  bool lines = zoomx>4096;

  CIRCBUF_FORBEGIN(Signal_T,circbuf,-zoomx+1,0)
    { 
      absavg+=fabs(*cbptr);
      avg+=*cbptr++;
      i2++;
      if(samples_used > samples_per_pixels)
      {
        //std::cerr<<"x:"<<i<< " y:"<<256 + (int)(avg/i2*128)<<" avg:"<<avg<<" i2:"<<i2<<"\n";
        
        if(lines)
        {
          int val=CLAMP((int)(absavg/i2*midym1*zoomy),0,midym1);
          for(int y=midy-val;y<=midy+val;y++)
            _outImage->operator()(i,y)=WHITE_RGBA;
        }
        else
          _outImage->operator()(i,CLAMP(midy-(int)(avg/i2*midym1*zoomy),1,sizey-1))=WHITE_RGBA;
        i++;
        i2=0;
        absavg=0;
        avg=0;
        samples_used-=samples_per_pixels;
      }
      samples_used++;
    }
  CIRCBUF_FOREND

}



