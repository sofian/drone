#ifndef GEAR_PUSHBUTTON_INCLUDED
#define GEAR_PUSHBUTTON_INCLUDED

#include "Gear.h"
#include "SignalType.h"

enum eButtonState {ON, OFF};

class Gear_PushButton : public Gear  
{
public:
  static const float  DEFAULT_VALUE;

  static const std::string SETTING_OFFVALUE;
  static const std::string SETTING_ONVALUE;
  static const std::string SETTING_ONESHOT;

  Gear_PushButton(Engine *engine, std::string name);
  virtual ~Gear_PushButton();

  void runVideo();
  
  void setState(eButtonState state);
  eButtonState getState(){return _state;}
  float getValue(){return _VALUE_OUT->type()->value();}

  bool ready();

protected:
  GearGui* createGearGui(QCanvas *canvas);//we want a PushButton so we overload
  void onUpdateSettings();
  void onPlugConnected(AbstractPlug *plug);
  void onPlugDisconnected(AbstractPlug* plug);

private:
  PlugOut<ValueType>* _VALUE_OUT;  
  
  bool _acceptHint;
  bool _lastWasOn;
  eButtonState _state;
  // used to protected a very short click from being lost by a mouse release before it is seen by runVideo
  bool _haveUnconsumedClick;
  bool _havePendingRelease;

};

#endif 
