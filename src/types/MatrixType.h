#ifndef __MATRIXTYPE_INCLUDED
#define __MATRIXTYPE_INCLUDED

#include "Matrix.h"
#include "ValueType.h"

template <class T>
class MatrixType : public AbstractType, public Matrix<T>
{
public:
  typedef typename Matrix<T>::size_type size_type;

  MatrixType(int width = 0,
             int height = 0, 
             T fillValue = T())
    : Matrix<T>(width, height)      
  {
    addSubType(_typeWidth);
    addSubType(_typeHeight);
    resize(width, height);
    std::fill(begin(), end(), fillValue);
  }

  void resize(size_type size)
  {
    Matrix<T>::resize(size);
  }

  void resize(size_type width, size_type height)
  {
    Matrix<T>::resize(width, height);
    _typeWidth.setValue(width);
    _typeHeight.setValue(height);
  }

  void fill(const T& fillValue)
  {
    std::fill(begin(), end(), fillValue);
  }

  const T* data() const {return &front();}
  T* data() {return &front();}

  virtual ~MatrixType() {}
  

protected:
  ValueType _typeWidth;
  ValueType _typeHeight;  
};

#endif //__MATRIXTYPE_INCLUDED


