#ifndef SIMPLEGRAPH_INCLUDED
#define SIMPLEGRAPH_INCLUDED

#include "error.h"
#include "Array.h"

template <class T>
class SimpleGraph : Array<T>
{
public:
  SimpleGraph(size_type n = 0) : Array<T>(n)
  {
    edges.resize(n,n);
    edges.fill(false);
  }
  
  ~SimpleGraph() {}

  void clear()
  {
    Array<T>::clear();
    edges.clear();
  }

  void resize(size_type n, T t = T());
  {
    Array<T>::resize(n, t);
    edges.resize(n, n);
    edges.fill(false);
  }
  
  void addEdge(int i, int j, bool twoWays = true)
  {
    edges(i,j) = true;
    if (twoWays)
      edges(j,i) = true;
  }
  
  bool hasEdge(int i, int j) { return edges(i,j); }
  
protected:
  Array2D<bool> edges;
};

#endif
