#ifndef SIMPLEGRAPH_INCLUDED
#define SIMPLEGRAPH_INCLUDED

#include "error.h"
#include "Array.h"
#include "Array2D.h"

template <class T>
class SimpleGraph : Array<T>
{
public:
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

  void resize(size_type n, T t = T());
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
