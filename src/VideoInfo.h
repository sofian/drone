#ifndef VIDEOINFO_INCLUDED
#define VIDEOINFO_INCLUDED

#include "Timing.h"

class VideoInfo
{

  static const float DEFAULT_FRAMERATE;

public:
  VideoInfo();
  void FrameRate(float framerate) {
    _FrameRate = framerate;
    _TimePerFrame = 1.0f / framerate;
  };

  Time_T timePerFrame() const {return _TimePerFrame;}

  float frameRate() const {return _FrameRate;}    

private:
  Time_T _TimePerFrame;
  float _FrameRate;
};

#endif
