#ifndef TA_CITYGRAPH_INCLUDED
#define TA_CITYGRAPH_INCLUDED

#include "SimpleGraph.h"
#include "Array2D.h"
#include <string>
#include <cmath>

#include <qdom.h>
#include <qfile.h>

struct TA_CityVertex
{
  float x;
  float y;
  float energy;
  std::vector<std::string> clipFileNames;

  TA_CityVertex() {}
  TA_CityVertex(float x, float y)
    : energy(0)
  { this->x = x; this->y = y; }
  void *data;
};

float distance(TA_CityVertex *a, TA_CityVertex *b);

typedef Array2D<bool> TA_Grid;
typedef Array2D<TA_CityVertex*> TA_CentroidGrid;

class TA_CityGraph : public SimpleGraph<TA_CityVertex*>
{
public:
  //  TA_CityGraph(size_type nHotSpots = 0, size_type nCentroids = 0);
  TA_CityGraph() {}
  TA_CityGraph(const std::string& filename);
  virtual ~TA_CityGraph() ;

  void update(TA_Grid *grid);

  virtual void load(const std::string& filename);

protected:
  TA_CentroidGrid _gridCentroids;
};

#endif
