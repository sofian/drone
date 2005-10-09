#include "SimpleGraph.h"

SimpleGraph::SimpleGraph(size_type n) : Array<SimpleVertex>(n)
{
  edges.resize(n,n);
  edges.fill(false);
}

void SimpleGraph::clear()
{
  Array<SimpleVertex>::clear();
  edges.clear();
}

void SimpleGraph::resize(size_type n, SimplePoint *t)
{
  Array<SimpleVertex>::resize(n, t);
  edges.resize(n, n);
  edges.fill(false);
}

void SimpleGraph::addEdge(int i, int j, bool twoWays)
{
  edges(i,j) = true;
  if (twoWays)
    edges(j,i) = true;
}

bool SimpleGraph::hasEdge(int i, int j)
{
  return edges(i,j);
}

