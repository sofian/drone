#ifndef GEAR_VIDEOOUTPUT_INCLUDED
#define GEAR_VIDEOOUTPUT_INCLUDED

#include "Gear.h"
#include "VideoType.h"

class VideoOutput;

class Gear_VideoOutput : public Gear  
{
public:

  Gear_VideoOutput(Engine *engine, std::string name);
  virtual ~Gear_VideoOutput();

  void prePlay();
  void postPlay();
  void init();
  void runVideo();


  PlugIn<VideoType>* VIDEO_IN(){return _VIDEO_IN;};

  bool ready();

private:
  static const std::string SETTING_XRES;
  static const std::string SETTING_YRES;
  static const std::string SETTING_FULLSCREEN;

  static const int DEFAULT_XRES;
  static const int DEFAULT_YRES;    
  static const bool DEFAULT_FULLSCREEN; 

  PlugIn<VideoType> *_VIDEO_IN;

  VideoOutput *_videoOutput;
  std::vector<std::string> _allOutputs;

};

#endif
