#ifndef GEAR_FREQBOXES_INCLUDED
#define GEAR_FREQBOXES_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"

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


  PlugIn<SignalType>* SIGNAL_IN(){return _SIGNAL_IN;};
  PlugOut<VideoTypeRGBA>* VIDEO_OUT(){return _VIDEO_OUT;};

  void init();
  bool ready();

protected:
  void onUpdateSettings();

private:

  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<SignalType> *_SIGNAL_IN;

  //local var

  MatrixType<RGBA> *_outImage; 
  RGBA *_outData;
  Rasterer *_rasterer;

  FrequencyAnalyser *_freqAnalyser;    



};

#endif
