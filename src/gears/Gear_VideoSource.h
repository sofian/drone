#ifndef GEAR_VIDEOSOURCE_INCLUDED
#define GEAR_VIDEOSOURCE_INCLUDED


#include "Gear.h"
#include "VideoType.h"
#include "SignalType.h"

#include <libmpeg3.h>

class Gear_VideoSource : public Gear
{
public:
  static const std::string SETTING_FILENAME;

  Gear_VideoSource(Engine *engine, std::string name);
  virtual ~Gear_VideoSource();

  void runVideo();
  void runAudio();

  PlugOut<VideoTypeRGBA>* VIDEO_OUT(){return _VIDEO_OUT;};
  PlugOut<SignalType>* AUDIO_OUT(){return _AUDIO_OUT;};

  bool ready();

protected:
  void onUpdateSettings();

private:

  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugOut<SignalType> *_AUDIO_OUT;
  mpeg3_t* _file;

  int _sizeX, _sizeY;
  long _nbFrames;

  //locals
  
  float *_audioBuffer;
  RGBA *_frame[1024];
  RGBA *_outData;

  //mmx
  unsigned long long int *_mmxImageIn;
  unsigned long long int *_mmxImageOut;

};

#endif
