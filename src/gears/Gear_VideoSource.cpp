#include <iostream>
#include "Gear_VideoSource.h"
#include "Engine.h"

#include "GearMaker.h"


Register_Gear(MAKERGear_VideoSource, Gear_VideoSource, "VideoSource");

const std::string Gear_VideoSource::SETTING_FILENAME = "Filename";

Gear_VideoSource::Gear_VideoSource(Engine *engine, std::string name) : Gear(engine, "VideoSource", name),
_File(NULL),
_SizeX(0),
_SizeY(0)
{    
  _VIDEO_OUT = (PlugOut<VideoType>*) addPlug(new PlugOut<VideoType>(this, "ImgOut"));       
  _AUDIO_OUT = (PlugOut<SignalType>*) addPlug(new PlugOut<SignalType>(this, "AudioOut"));       

  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    
}

Gear_VideoSource::~Gear_VideoSource()
{
  if (_File!=NULL)
    mpeg3_close(_File);

}

bool Gear_VideoSource::ready()
{
  return(_VIDEO_OUT->connected() || _AUDIO_OUT->connected());
}

void Gear_VideoSource::onUpdateSettings()
{
  char tempstr[1024];

  strcpy(tempstr,_settings.get(SETTING_FILENAME)->valueStr().c_str());

  std::cout << "opening movie : " << tempstr << std::endl;

  if (_File!=NULL)
    mpeg3_close(_File);

  _File = mpeg3_open(tempstr);    

  if (_File==NULL)
  {
    std::cout << "error opening movie : " << tempstr << std::endl;
    return;
  }


  _SizeX = mpeg3_video_width(_File, 0);
  _SizeY = mpeg3_video_height(_File, 0);


  std::cout << "movie size X : " << _SizeX << std::endl;
  std::cout << "movie size Y : " << _SizeY << std::endl;

  std::cout << "numframes : " << mpeg3_video_frames(_File, 0) << std::endl;
  std::cout << "movie samplerate : " << mpeg3_sample_rate(_File,0) << std::endl;


  for (int i=0;i<_SizeY-1;i++)
    _Frame[i] = (RGBA*) malloc(_SizeX * sizeof(RGBA));

  //from the doc :
  //You must allocate 4 extra bytes in the last output_row. This is scratch area for the MMX routines.
  _Frame[_SizeY-1] = (RGBA*) malloc((_SizeX * sizeof(RGBA)) + 4);

  _VIDEO_OUT->type()->canvas()->allocate(_SizeX, _SizeY);
}

void Gear_VideoSource::runVideo()
{
  if (_File==NULL)
    return;

  _image = _VIDEO_OUT->type()->canvas();

  mpeg3_read_frame(_File, (unsigned char**)_Frame, 0, 0, _SizeX, _SizeY, _SizeX, _SizeY, MPEG3_RGBA8888, 0);

  //for(int y=0;y<_SizeY;y++)
  //    memcpy(&(_image->_Data[y*_SizeX]), _Frame[y], sizeof(RGBA) * _SizeX);

  register int mmxCols=(_SizeX)/2;
  register int index;    

  for (int y=0;y<_SizeY;y++)
  {
    _mmxImageIn = (double*)_Frame[y];        
    _mmxImageOut = (double*)&(_image->_data[y*_SizeX]);

    for (index=0;index<mmxCols;index++)
    {
      __asm__ volatile (
                       "\n\t movq %1,%%mm0        \t# (u) load imageIn"
                       "\n\t movq %%mm0,%0        \t# (u) store result "
                       : "=m" (_mmxImageOut[index])  // this is %0, output
                       : "m"  (_mmxImageIn[index]) // this is %1, image A                 
                       );
    }            

  }
  __asm__("emms" : : );

}

void Gear_VideoSource::runAudio()
{
  _audioBuffer = _AUDIO_OUT->type()->buffer();

  //mpeg3_read_audio(_File, signal, NULL, 1, 128, 0);
}


