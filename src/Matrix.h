#ifndef __MATRIX_INCLUDED
#define __MATRIX_INCLUDED

#include "error.h"
#include <vector>

/**
 * This class is a general-purpose structure for accessing data structured as
 * a two-dimensional array. The memory it contains is contiguous as the class
 * derives from the STL <code>vector</code> class. The main advantage of using
 * a <code>Matrix</code> over a <code>vector</code> is that it offers a nice
 * interface for accessing data in a "by-row" fashion.
 *
 * @author Jean-Sébastien Senécal, Mathieu Guindon
 * @version %I% %G%
 */
template <class T>
class Matrix : public std::vector<T>
{
public:
  //! Inner type definitions.
  typedef typename std::vector<T>::size_type size_type;
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::reference reference;
  typedef typename std::vector<T>::pointer pointer;

public:
  /**
   * Default constructor.
   */
  Matrix() : _width(0), _height(0) {}

  /**
   * Constructor.
   *
   * @param width the wished number of columns
   * @param height the wished number of rows
   */
  Matrix(size_type width,
         size_type height)    
    : _width(0), _height(0)
  {
    resize(width, height);
  }

  //! Destructor.
  virtual ~Matrix() {}

  /**
   * Returns the number of rows.
   *
   * @return the height
   */
  size_type height() const { return _height; }

  /**
   * Returns the number of columns.
   *
   * @return the width
   */
  size_type width() const { return _width; }

  /**
   * Returns a reference to the value at coordinates <code>(x,y)</code>.
   *
   * @param x the column index
   * @param y the row index
   * @return the value
   */
  reference operator()(size_type x, size_type y) const
  {
    ASSERT_ERROR(x < width());
    ASSERT_ERROR(y < height());
    return _rows[y][x];
  }

  /**
   * Returns a reference to the value at coordinates <code>(x,y)</code>.
   *
   * @param x the column index
   * @param y the row index
   * @return the value
   */
  reference get(size_type x, size_type y) const
  {
    ASSERT_ERROR(x < width());
    ASSERT_ERROR(y < height());
    return _rows[y][x];
  }

  /**
   * Returns a reference to the vector of rows.
   *
   * @return the reference to the row vector
   */
  std::vector<pointer>& rows() const { return _rows; }

  /**
   * Resizes the matrix as a vector. Calling this sets <code>height</code> to <code>1</code>
   * and <code>width</code> to <code>size</code>.
   *
   * @param size the new size
   */
  void resize(size_type size)
  {
    std::vector<T>::resize(size);
    _width=size;
    _height=1;

    _rows.resize(1);
    _rows[0] = &front();
  }

  /**
   * Resizes the matrix to new width/height.
   *
   * @param width the new width
   * @param height the new height
   */
  void resize(size_type width, size_type height)
  {
    if (width==_width && height==_height)
      return;
    
    std::vector<T>::resize(width*height);
    _width = width;
    _height = height;
  
    // Efficient setting of the rows pointer.
    _rows.resize(height);
    pointer iterData = &front();
    for (typename std::vector<pointer>::iterator it = _rows.begin();
          it != _rows.end(); ++it, iterData += width)
      *it = iterData;
  }               

  /**
   * Returns a pointer to the first element of a row (const version).
   *
   * @param y the row number
   * @return a pointer to the start of the row
   */
  const pointer row(size_type y) const
  {
    ASSERT_ERROR(y < height());
    return _rows[y];
  }

  /**
   * Returns a pointer to the first element of a row.
   *
   * @param y the row number
   * @return a pointer to the start of the row
   */
  pointer row(size_type y)
  {
    ASSERT_ERROR(y < height());
    return _rows[y];
  }

protected:
  //! The number of columns.
  size_type _width;

  //! The number of rows.
  size_type _height;

  //! The precomputed array of row pointers.
  std::vector<pointer> _rows;
};

#endif
