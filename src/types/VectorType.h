#ifndef __VECTORTYPE_INCLUDED
#define __VECTORTYPE_INCLUDED

#include <vector>
#include "ValueType.h"

template <class T>
class VectorType : public AbstractType, public std::vector<T>
{
public:
  VectorType(int size = 0,
             T fillValue = T())
    : _fillValue(fillValue)
  {
    addSubType(_typeSize);
    resize(size);
    std::fill(begin(), end(), _fillValue);
  }

  void resize(size_t size)
  {
    std::vector<T>::resize(size);
    _typeSize.setValue(size);
  }

  void fill(const T& fillValue)
  {
    std::fill(begin(), end(), fillValue);
  }

  const T* data() const {return &front();}
  T* data() {return &front();}
  

  virtual ~VectorType() {}
  
protected:
  ValueType _typeSize;
  T _fillValue;
};

#endif //__VECTORTYPE_INCLUDED


