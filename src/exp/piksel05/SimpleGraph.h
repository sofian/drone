#ifndef SIMPLEGRAPH_INCLUDED
#define SIMPLEGRAPH_INCLUDED

#include "error.h"
#include "Array.h"

struct SimpleVertex
{
  // va contenir les informations sur un point
  float x, y; // position
  void *data;
};

class SimpleGraph : Array<SimpleVertex>
{
public:
  SimpleGraph(size_type n = 0);
  ~SimpleGraph() {}

  void clear();
  void resize(size_type n, SimpleVertex t = SimpleVertex());
  void addEdge(int i, int j, bool twoWays = true);
  bool hasEdge(int i, int j);
  
protected:
  Array2D<bool> edges;
};
