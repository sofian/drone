#ifndef GEAR_VideoSource_INCLUDED
#define GEAR_VideoSource_INCLUDED


#include "Gear.h"

#include <libmpeg3.h>

class Gear_VideoSource : public Gear
{
public:
  static const std::string SETTING_FILENAME;

  Gear_VideoSource(Engine *engine, std::string name);
  virtual ~Gear_VideoSource();
        
  void runVideo();
  void runAudio();
    
  PlugOut<VideoType>* VIDEO_OUT(){return _VIDEO_OUT;};
  PlugOut<SignalType>* AUDIO_OUT(){return _AUDIO_OUT;};

  bool ready();

protected:
  void onUpdateSettings();

private:

  PlugOut<VideoType> *_VIDEO_OUT;
  PlugOut<SignalType> *_AUDIO_OUT;
  mpeg3_t* _File;

  int _SizeX, _SizeY;
    
  //locals
  Canvas *_image;
  float *_audioBuffer;
  RGBA *_Frame[1024];

  //mmx
  double *_mmxImageIn;
  double *_mmxImageOut;

};

#endif
