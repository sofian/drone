#ifndef __VALUETYPE_INCLUDED
#define __VALUETYPE_INCLUDED

#include "AbstractType.h"

class ValueType : public AbstractType
{
public:  
  ValueType(float v=0.0f) : _value(v) {}  
  virtual ~ValueType() {}
  
  virtual std::string name() const { return "ValueType"; }
  virtual QColor color() const { return QColor(141, 222, 46); }

  void setValue(float value) { _value = value; }
  float value() const { return _value; }
  operator float () const {return _value;}  
  
                        
private:
  float _value;
};

#endif






