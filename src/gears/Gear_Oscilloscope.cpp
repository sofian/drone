#include "Gear_Oscilloscope.h"
#include "Engine.h"
#include <iostream>
#include "GearMaker.h"

#include <GL/gl.h>              
#include <GL/glu.h>

Register_Gear(MAKERGear_Oscilloscope, Gear_Oscilloscope, "Oscilloscope")

Gear_Oscilloscope::Gear_Oscilloscope(Engine *engine, std::string name) : Gear(engine, "Oscilloscope", name)
{
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "In"));
  addPlug(_ZOOM_X = new PlugIn<ValueType>(this, "ZoomX", new ValueType(44100,512,192400)));
  addPlug(_ZOOM_Y = new PlugIn<ValueType>(this, "ZoomY", new ValueType(1,0,2)));
  addPlug(_SIZE_X = new PlugIn<ValueType>(this, "SizeX", new ValueType(256,32,768)));
  addPlug(_SIZE_Y = new PlugIn<ValueType>(this, "SizeY", new ValueType(256,32,768)));

  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "Out"));

  circbuf = new CircularBufferSignal(0.0f,192400);
}

Gear_Oscilloscope::~Gear_Oscilloscope()
{
  delete circbuf;
}

bool Gear_Oscilloscope::ready()
{
  return(_AUDIO_IN->connected());// && _VIDEO_OUT->connected());
}

void Gear_Oscilloscope::runAudio()
{
  int sizey = CLAMP(_SIZE_Y->type()->value(),32,512);
  int sizex = CLAMP(_SIZE_X->type()->value(),32,1440);
  int midy = sizey>>1;
  int midym1 = midy-1;

  _outImage = _VIDEO_OUT->type()->image();
  _outImage->resize(sizex,sizey);
  _outData = _outImage->data();

  float zoomy = _ZOOM_Y->type()->value();
  int zoomx = _ZOOM_X->type()->value();
  int signal_blocksize = Engine::signalInfo().blockSize();

  // ? debug circularbuffer please .. can't get total buffer ?
  zoomx=CLAMP(zoomx,signal_blocksize,191399);

  MatrixType<float> buffer = _AUDIO_IN->type()->buffer();

  circbuf->append(buffer.data(),buffer.size());
  _outImage->fill(BLACK_RGBA);

  float samples_per_pixels = zoomx / (float)sizex;
  float samples_used=0, avg=0,absavg=0;
  int i=0,i2=0;

//  std::cerr<<"spp : "<<samples_per_pixels<< " zoomx :"<<zoomx<<"\n";
  
  bool lines = zoomx>4096;

  CIRCBUF_SIGNAL_T_FORBEGIN(circbuf,-zoomx,0)
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
        i++;i2=0;
        absavg=0;avg=0;
        samples_used-=samples_per_pixels;
      }
      samples_used++;
      
      
    }
  CIRCBUF_SIGNAL_T_FOREND

}



