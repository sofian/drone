#ifndef GEAR_VIDEOINPUT_INCLUDED
#define GEAR_VIDEOINPUT_INCLUDED


#include "Gear.h"

extern "C" {

#include <linux/videodev.h>
#define HAVE_STDINT_H
#include <liblavrec.h>
#undef HAVE_STDINT_H
}

class Gear_VideoInput : public Gear
{
public:
  static const std::string SETTING_DEVICE;
  static const std::string DEFAULT_DEVICE;

  Gear_VideoInput(Engine *engine, std::string name);
  virtual ~Gear_VideoInput();

  void runVideo();
  void init();
  void prePlay();    

  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};

  bool ready();

protected:
  void onUpdateSettings();
  static void videoCallback(uint8_t *video, long size, long count);

private:

  void initInputDevice();
  void resetInputDevice();

  PlugVideoOut *_VIDEO_OUT;

  int _sizeX, _sizeY;

  //v4l
  int _device;
  struct video_capability _vidCap;
  struct video_window _vidWin;
  struct video_picture _vidPic;
  struct video_clip _vidClips[32];

  //mmap
  struct video_mbuf _vidMBuf;    
  struct video_mmap _vidMMap;    
  struct video_buffer _vidBuf;
  unsigned char *_bufferBGRA;


  //locals
  unsigned char *_outData;
  unsigned char *_tempOutData;
  unsigned char *_tempInData;


  //mmx
  double *_mmxImageIn;
  double *_mmxImageOut;

  lavrec_t *_lavrecInfo;
  static unsigned char *_data;

};

#endif
