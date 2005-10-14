#ifndef CITYGRAPH_INCLUDED
#define CITYGRAPH_INCLUDED

#include "SimpleGraph.h"
#include "Array2D.h"

struct CityVertex
{
  float x;
  float y;
  float energy;
  std::vector<std::string> clipFileNames;
  
  void *data;
};

float distance(CityVertex *a, CityVertex *b)
{
  float xdiff = (a->x - b->x);
  float ydiff = (a->y - b->y);
  return sqrt( xdiff*xdiff + ydiff*ydiff );
}

typedef Array2D<bool> Grid;
typedef Array2D<CityVertex*> CentroidGrid;

class TA_CityGraph : public SimpleGraph<CityVertex*>
{
public:
  TA_CityGraph(size_type nHotSpots, size_type nCentroids);
  ~TA_CityGraph() {}

  void update(Grid *grid);

protected:
  CentroidGrid _gridCentroids;
};

#endif
