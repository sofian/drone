#ifndef GEARCONVERTER_INCLUDED
#define GEARCONVERTER_INCLUDED

#include "Gear.h"

template <class TypeIn, class TypeOut>
class GearConverter : public Gear
{
public:
  GearConverter(Engine *engine, std::string type, std::string name)
    : Gear(engine, type, name)
  {
    addPlug(_PLUG_IN  = new PlugIn<TypeIn>(this, "In"));
    addPlug(_PLUG_OUT = new PlugOut<TypeOut>(this, "Out"));
  }
  
  virtual ~GearConverter() {}

  bool canConvert(const AbstractType& fromType, const AbstractType& toType,
                  std::pair<const AbstractPlug*, const AbstractPlug*>& plugs) const
  {
    if (_PLUG_IN->type()->name()  == fromType.name() &&
        _PLUG_OUT->type()->name() == toType.name() )
    {
      plugs.first  = _PLUG_IN;
      plugs.second = _PLUG_OUT;
      return true;
    }
    else
    {
      plugs.first = plugs.second = 0;
      return false;
    }
  }

  virtual void convert() = 0;
  
  void runAudio() { convert(); }
  void runVideo() { convert(); }
  
  bool ready() { return _PLUG_OUT->connected(); }
  
protected:
  PlugIn<TypeIn> *_PLUG_IN;
  PlugOut<TypeOut> *_PLUG_OUT;
};


#endif
