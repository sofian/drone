#ifndef GEAR_FREQBOXES_INCLUDED
#define GEAR_FREQBOXES_INCLUDED


#include "Gear.h"

#include "FrequencyAnalyzer.h"
#include "Rasterer.h"

class Gear_FreqBoxes : public Gear
{
public:

  static const std::string SETTING_SIZE_X;
  static const std::string SETTING_SIZE_Y;

  Gear_FreqBoxes(Engine *engine, std::string name);
  virtual ~Gear_FreqBoxes();

  void runVideo();


  PlugSignalIn* SIGNAL_IN(){return _SIGNAL_IN;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};

  void init();
  bool ready();

protected:
  void onUpdateSettings();

private:

  PlugVideoOut *_VIDEO_OUT;
  PlugSignalIn *_SIGNAL_IN;

  //local var

  Video_T *_outImage; 
  RGBA *_outData;
  Rasterer *_rasterer;

  FrequencyAnalyser *_freqAnalyser;    



};

#endif
