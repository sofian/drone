#ifndef ENUMTYPE_INCLUDED
#define ENUMTYPE_INCLUDED

#include "AbstractType.h"
#include <vector>

class EnumType : public AbstractType
{
public:
  EnumType(int size = 0, int defaultValue=0)
  {
    _names.resize(size);
    setValue(defaultValue);
  }
  
  virtual ~EnumType() {}
  
  virtual std::string name() const { return "EnumType"; }
  virtual QColor color() const { return QColor(238,19,19); }
  
  void setValue(int value) {
    ASSERT_ERROR(_names.empty() || (0 <= value && value < _names.size()));
    _value = value;
  }
  int value() const { return _value; }
  
  operator int () const {return _value;}
  
  void setName(int value, const std::string& name) { _names[value] = name; }
  const std::string& name(int value) const { return _names[value]; }
  
private:
  int _value;
  std::vector<std::string> _names;
};

#endif
