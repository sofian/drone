#include <iostream>
#include "Gear_VideoSource.h"
#include "Engine.h"

#include "GearMaker.h"


Register_Gear(MAKERGear_VideoSource, Gear_VideoSource, "VideoSource");

const std::string Gear_VideoSource::SETTING_FILENAME = "Filename";

Gear_VideoSource::Gear_VideoSource(Engine *engine, std::string name) : 
  Gear(engine, "VideoSource", name),
  _file(NULL),
  _sizeX(0),
  _sizeY(0),
  _nbFrames(0)
                                                                            
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOut"));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "AudioOut"));

  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    
}

Gear_VideoSource::~Gear_VideoSource()
{
  if (_file!=NULL)
    mpeg3_close(_file);

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

  if (_file!=NULL)
    mpeg3_close(_file);

  _file = mpeg3_open(tempstr);    
  
  if (_file==NULL)
  {
    std::cout << "error opening movie : " << tempstr << std::endl;
    return;
  }

  //mpeg3_set_cpus(_file, 1);
  //mpeg3_set_mmx(_file, 1);


  _sizeX = mpeg3_video_width(_file, 0);
  _sizeY = mpeg3_video_height(_file, 0);


  std::cout << "movie size X : " << _sizeX << std::endl;
  std::cout << "movie size Y : " << _sizeY << std::endl;

  _nbFrames =  mpeg3_video_frames(_file, 0);
  std::cout << "numframes : " << _nbFrames << std::endl;
  std::cout << "movie samplerate : " << mpeg3_sample_rate(_file,0) << std::endl;


  for (int i=0;i<_sizeY-1;i++)
    _frame[i] = (RGBA*) malloc(_sizeX * sizeof(RGBA));

  //from the doc :
  //You must allocate 4 extra bytes in the last output_row. This is scratch area for the MMX routines.
  _frame[_sizeY-1] = (RGBA*) malloc((_sizeX * sizeof(RGBA)) + 4);

  _VIDEO_OUT->type()->resize(_sizeX, _sizeY);
}

void Gear_VideoSource::runVideo()
{
  if (_file==NULL)
    return;

  //_image = _VIDEO_OUT->type().image();
  //_outData = _image.data();
  mpeg3_read_frame(_file, (unsigned char**)_frame, 0, 0, _sizeX, _sizeY, _sizeX, _sizeY, MPEG3_RGBA8888, 0);

  _outData = _VIDEO_OUT->type()->data();

  for(int y=0;y<_sizeY;y++)
    memcpy(&_outData[y*_sizeX], _frame[y], sizeof(RGBA) * _sizeX);

  //loop
  if (mpeg3_get_frame(_file,0) >= _nbFrames)  
    mpeg3_set_frame(_file, 0, 0);
  

  //register int mmxCols=(_sizeX)/2;
  //register int index;    

  /*   _mmxImageIn = (unsigned long long int*) _frame;        */
  /*   _mmxImageOut =(unsigned long long int*) _image.data(); */
  /*                                                          */


  /*                                                                            */
  /*   for (int y=0;y<_sizeY;y++)                                               */
  /*   {                                                                        */
  /*     _mmxImageIn = (unsigned long long int*)_frame[y];                      */
  /*     _mmxImageOut = (unsigned long long int*)(_outData + y*_sizeX);         */
  /*                                                                            */
  /*     for (index=0;index<mmxCols;index++)                                    */
  /*     {                                                                      */
  /*       __asm__ volatile (                                                   */
  /*                        "\n\t movq %1,%%mm0        \t# (u) load imageIn"    */
  /*                        "\n\t movq %%mm0,%0        \t# (u) store result "   */
  /*                        : "=m" (_mmxImageOut[index])  // this is %0, output */
  /*                        : "m"  (_mmxImageIn[index]) // this is %1, image A  */
  /*                        );                                                  */
  /*     }                                                                      */
  /*    _mmxImageIn++;          */
  /*    _mmxImageOut += _sizeX; */
  /*   }                     */
  /*   __asm__("emms" : : ); */

}

void Gear_VideoSource::runAudio()
{
  //  _audioBuffer = _AUDIO_OUT->type()->type()->data();

  //mpeg3_read_audio(_file, signal, NULL, 1, 128, 0);
}


