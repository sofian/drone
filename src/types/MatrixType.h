#ifndef __MATRIXTYPE_INCLUDED
#define __MATRIXTYPE_INCLUDED

#include "ValueType.h"

template <class T>
class MatrixType : public AbstractType
{
public:
  MatrixType(int width = 0,
             int height = 0, 
             T fillValue = T())
    : _data(0)
    {
      addSubType(_width);
      addSubType(_height);
      resize(width, height);
      fill(fillValue);
    }
  
  virtual ~MatrixType() {}

  const ValueType& width() const { return _width; }
  const ValueType& height() const { return _height; }

  int size() const { return _size; }
  int sizeX() const { return _sizeX; }
  int sizeY() const { return _sizeY; }

  T& operator[](int i) const { return _data[i]; }
  T operator()(int x, int y) const { return _data[y*_sizeX+x]; }
  
  T* data() { return _data; }
  
  virtual std::string name() const {return "!error! UndefinedMatrixType";} 
  virtual QColor color() const {  return QColor(127,127,127); }
  
  void resize(const ValueType& width, const ValueType &height) 
  {
    resize(width.value(), height.value());
  }

  void resize(int sizeX, int sizeY=1) 
  {
    _height.setValue(sizeX);
    _width.setValue(sizeY);
    _sizeX = sizeX;
    _sizeY = sizeY;
    allocate(sizeX*sizeY);
  }
  
  void fill(T value)
  {
    for (T *it = _data; it != _data+_size; ++it)
      *it++ = value;
  }

private:
  void allocate(size_t size)
  {
    _data = (T*)realloc(_data, size*sizeof(T));
    _size = size;
  }

  ValueType _width;
  ValueType _height;
  int _size;
  int _sizeX;
  int _sizeY;
  T *_data;
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


