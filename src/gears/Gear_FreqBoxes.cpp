#include "Gear_FreqBoxes.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_FreqBoxes, Gear_FreqBoxes, "FreqBoxes")

const std::string Gear_FreqBoxes::SETTING_SIZE_X = "SizeX";
const std::string Gear_FreqBoxes::SETTING_SIZE_Y = "SizeY";


Gear_FreqBoxes::Gear_FreqBoxes(Engine *engine, std::string name) : Gear(engine, "FreqBoxes", name),
_rasterer(NULL),
_freqAnalyser(NULL)
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "Img"));
  addPlug(_SIGNAL_IN = new PlugIn<SignalType>(this, "Signal"));

  _settings.add(Property::INT, SETTING_SIZE_X)->valueInt(256);    
  _settings.add(Property::INT, SETTING_SIZE_Y)->valueInt(256);    

  updateSettings();

}

Gear_FreqBoxes::~Gear_FreqBoxes()
{

}

void Gear_FreqBoxes::init()
{    
  if (_freqAnalyser!=NULL)
    delete _freqAnalyser;

  _freqAnalyser = new FrequencyAnalyser(Engine::signalInfo().blockSize());    
  _rasterer = new Rasterer();
  _rasterer->setImage(_VIDEO_OUT->type());

}

void Gear_FreqBoxes::onUpdateSettings()
{    
  _VIDEO_OUT->type()->resize(_settings.get(SETTING_SIZE_X)->valueInt(), _settings.get(SETTING_SIZE_Y)->valueInt());
}


bool Gear_FreqBoxes::ready()
{
  return(_VIDEO_OUT->connected() && _SIGNAL_IN->connected());
}

void Gear_FreqBoxes::runVideo()
{    
  _outImage = _VIDEO_OUT->type();
  memset(_outImage->data(), 0, _outImage->size()*sizeof(RGBA));
  _outData = _outImage->data();
  int sizeX = _outImage->width();
  int sizeY = _outImage->height();

  //Signal_T *bufferin = _SIGNAL_IN->type()->data();

  //_freqAnalyser->performAnalysis(bufferin);

  int nbBoxes = 16;
  int boxSize = (sizeX / nbBoxes) / 2;
  //int bandInterval = 220 / 25;
  int x=sizeX/2;
  int y=sizeY/2;
  int size=sizeX/2;
  float energyOfBand=0.0f;
  unsigned char color=0;

  for (int i=0;i<nbBoxes;i++)
  {
    //energyOfBand = _freqAnalyser->getBand(i*bandInterval, i*bandInterval + bandInterval);
    color = (unsigned char) (energyOfBand*10*255.0f);
    _rasterer->setColor(color, color/2, color/4);
    _rasterer->rect(x - size, y - size, x+size, y+size, true);
    size -= boxSize;

  }

}
