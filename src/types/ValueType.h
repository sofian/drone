#ifndef __VALUETYPE_INCLUDED
#define __VALUETYPE_INCLUDED

#include "AbstractType.h"

class ValueType : public AbstractType
{
public:
  ValueType(float defaultValue=0.0f,
            float minValue=-1.0f,
            float maxValue=1.0f)
    : _value(defaultValue),
      _minValue(minValue),
      _maxValue(maxValue)
    {}
  
  virtual ~ValueType() {}
  
  virtual std::string name() const { return "ValueType"; }
  virtual QColor color() const { return QColor(157, 208, 50); }
  
  void setValue(float value) { _value = value; }
  float value() const { return _value; }
  
  operator float () const {return _value;}
  
  void setMinValue(float minValue) { _minValue = minValue; }
  void setMaxValue(float maxValue) { _maxValue = maxValue; }

  float minValue() const { return _minValue; }
  float maxValue() const { return _maxValue; }

private:
  float _value, _minValue, _maxValue;
};

#endif






