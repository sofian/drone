#ifndef GEAR_SLIDER_INCLUDED
#define GEAR_SLIDER_INCLUDED

#include "Gear.h"
#include "SignalType.h"


class Gear_Slider : public Gear  
{
public:
  static const float  DEFAULT_VALUE;

  static const std::string SETTING_LOWERBOUND;
  static const std::string SETTING_HIGHERBOUND;

  Gear_Slider(Engine *engine, std::string name);
  virtual ~Gear_Slider();

  void save(QDomDocument &doc, QDomElement &gearElem);
  void load(QDomElement &gearElem);

  void runAudio();

  void setValue(float  value);
  float getValue(){return _VALUE_OUT->type()->value();};

  bool ready();

protected:
  GearGui* createGearGui(QCanvas *canvas);//we want a slider so we overload
  void onUpdateSettings();
  void onPlugConnected(AbstractPlug *plug);
  void onPlugDisconnected(AbstractPlug* plug);

private:
  PlugOut<ValueType>* _VALUE_OUT;  
  
  bool _acceptHint;

};

#endif 
