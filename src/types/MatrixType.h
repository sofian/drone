#ifndef __MATRIXTYPE_INCLUDED
#define __MATRIXTYPE_INCLUDED

#include "ValueType.h"

template <class T>
class MatrixType : public AbstractType, public std::vector<T>
{
public:
  MatrixType(int width = 0,
             int height = 0, 
             T fillValue = T())
    : _width(width),
      _height(height)
    {
      addSubType(_width);
      addSubType(_height);
      fill(fillValue);
    }
  
  virtual ~MatrixType() {}

  const ValueType& width() const { return _width; }
  const ValueType& height() const { return _height; }

  int sizeX() const { return static_cast<int>(_width); }
  int sizeY() const { return static_cast<int>(_height); }
  
  T operator()(int x, int y) const { return operator[](x*static_cast<int>(_width)+y); }
  
  T* data() { return &operator[](0); }
  
  virtual std::string name() const {return "!error! UndefinedMatrixType";} 
  virtual QColor color() const {  return QColor(127,127,127); }
  
  void resize(size_t size)
  {
    std::vector<T>::resize(size);
  }

  void resize(int width, int height) 
  {
    _height.setValue(height);
    _width.setValue(width);
    resize(width*height);
  }
  
  void resize(const ValueType& width, const ValueType &height) 
  {
    _width = width;
    _height = height;
    resize((int)(height.value()*width.value()));
  }

  void fill(T value)
  {
    std::fill(begin(), end(), value);
  }

private:
  ValueType _width;
  ValueType _height;
};

/* template<>                                   */
/* std::string MatrixType<float>::name() const; */
/*                                              */
/* template<>                                   */
/* QColor MatrixType<float>::color() const;     */
/*                                              */
/* template<>                                   */
/* std::string MatrixType<char>::name() const;  */
/*                                              */
/* template<>                                   */
/* QColor MatrixType<char>::color() const;      */


#endif //__MATRIXTYPE_INCLUDED


