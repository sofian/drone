#ifndef __ABSTRACTTYPE_INCLUDED
#define __ABSTRACTTYPE_INCLUDED

#include <qcolor.h>
#include <string>
#include <vector>

class AbstractType
{
public:
  AbstractType() {}
  virtual ~AbstractType() {}

  virtual std::string name() const = 0;
  virtual QColor color() const = 0;

  int nSubTypes() const { return _subTypes.size();}
  const AbstractType* getSubType(int i) const { return _subTypes[i]; }
  const AbstractType* addSubType(const AbstractType &subType)
  {
    _subTypes.push_back(&subType);
    return &subType;
  }

protected:
  std::vector<const AbstractType*> _subTypes;
};


#endif //__ABSTRACTTYPE_INCLUDED

