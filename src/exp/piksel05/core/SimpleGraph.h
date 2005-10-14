#ifndef SIMPLEGRAPH_INCLUDED
#define SIMPLEGRAPH_INCLUDED

#include "error.h"
#include "Array.h"
#include "Array2D.h"

template <class T>
class SimpleGraph : public Array<T>
{
public:
  //! Inner type definitions.
  typedef typename std::vector<T>::size_type size_type;
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::reference reference;
  typedef typename std::vector<T>::const_reference const_reference;
  typedef typename std::vector<T>::pointer pointer;

  
public:
  // using (as suggested by Norm)
  using std::vector<T>::end;
  using std::vector<T>::begin;
  using std::vector<T>::front;

  SimpleGraph(size_type n = 0) : Array<T>(n)
  {
    _edges.resize(n,n);
    _edges.fill(false);
  }
  
  ~SimpleGraph() {}

  void clear()
  {
    Array<T>::clear();
    _edges.clear();
  }

  void resize(size_type n, T t = T())
  {
    Array<T>::resize(n, t);
    _edges.resize(n, n);
    _edges.fill(false);
  }
  
  void addEdge(int i, int j, bool twoWays = true)
  {
    _edges(i,j) = true;
    if (twoWays)
      _edges(j,i) = true;
  }
  
  bool hasEdge(int i, int j) { return _edges(i,j); }
  
protected:
  Array2D<bool> _edges;
};

#endif
