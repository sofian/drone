#ifndef GEAR_IMAGE_INCLUDED
#define GEAR_IMAGE_INCLUDED


#include "Gear.h"
#include "VideoType.h"

class PlugVideo;

class Gear_Image : public Gear
{
public:
  static const std::string SETTING_FILENAME;

  Gear_Image(Engine *engine, std::string name);
  virtual ~Gear_Image();

  void onUpdateSettings();
  void runVideo();
  bool ready();

private:

  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;



};

#endif
