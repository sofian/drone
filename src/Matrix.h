#ifndef __MATRIX_INCLUDED
#define __MATRIX_INCLUDED

#include <vector>

template <class T>
class Matrix : public std::vector<T>
{
public:
  typedef typename std::vector<T>::size_type size_type;
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::reference reference;
  typedef typename std::vector<T>::pointer pointer;

public:
  Matrix()
  {
    resize(0,0);
  }

  Matrix(size_type width,
         size_type height)
  {
    resize(width, height);
  }
  virtual ~Matrix() {}
  
  size_type height() const { return _height; }
  size_type width() const { return _width; }

  reference operator[](size_type x, size_type y) const { return _rows[y][x]; }
  reference get(size_type x, size_type y)  const { return _rows[y][x]; }
  
  std::vector<pointer>& rows() const { return _rows; }
  
  void resize(size_type size)
  {
    std::vector<T>::resize(size);
    _width=size;
    _height=1;
  }

  void resize(size_type width, size_type height)
  {
    if (width==_width && height==_height)
      return;
      
    std::vector<T>::resize(width*height);
    _width = width;
    _height = height;
  
    // Efficient setting of the rows pointer.
    _rows.resize(height);
    pointer iterData = data();
    for (typename std::vector<pointer>::iterator it = _rows.begin();
          it != _rows.end(); ++it, iterData += width)
      *it = iterData;
  }               

  const pointer data() const {return &front();}
  pointer data() {return &front();}

  const pointer row(int i) const { return &(*getRow(i)); }
  pointer row(int i) { return &(*getRow(i)); }

protected:
  size_type _width, _height;
  std::vector<pointer> _rows;
};

#endif
