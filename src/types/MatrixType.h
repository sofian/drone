#ifndef __MATRIXTYPE_INCLUDED
#define __MATRIXTYPE_INCLUDED

#include "ValueType.h"

template <class T>
class MatrixType : public AbstractType, public std::vector<T>
{
public:
  MatrixType() : _height(0), _width(0)
  {
    addSubType(_height);
    addSubType(_width);
  }
  
  virtual ~MatrixType() {}


  //int size() const { return _size; }
  const ValueType& height() const { return _height; }
  const ValueType& width() const { return _width; }
    
  T operator()(int row, int col) const { return operator[](row*_width.getValue()+col); }
  
  T* data() { return &operator[](0); }

  virtual std::string name() const {return "error! UndefinedMatrixType";} 
  virtual QColor color() const {  return QColor(0,0,0); }
  
  void resize(size_t size)
  {
    std::vector<T>::resize(size);
  }

  void resize(int height, int width) 
  {
    _height.setValue(height);
    _width.setValue(width);
    resize(height*width);
  }
  
  void resize(const ValueType &height, const ValueType& width) 
  {
    _height = height;
    _width = width;
    resize((int)(height.value()*width.value()));
  }

private:
  ValueType _height;  
  ValueType _width;
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


