#include "TA_CityGraph.h"

// foreach i
//   E_i = 0

// loop

//   // add energy where people are
//   foreach activated centroid c // centroids distribute their energy to close hot-spots
//     sum = 0
//     foreach i
//       a_i = 1 / d(h_i,c) // or 1 / d^2(h_i,c)
//       sum += a_i
//     foreach i
//       a_i /= sum
//       E_i += a_i

//   // lower energy everywhere
//   foreach i
//     E_i -= decay

//   // lower energy where the agent is (p being the agent's position)
//   E_p -= decay_agent

TA_CityGraph::TA_CityGraph(size_type nHotSpots, size_type nCentroids)
 : SimpleGraph<CityVertex*>(n),
   _gridCentroids(nCentroids)
{
  
}

TA_CityGraph::TA_CityGraph(const std::string& filename)
{

}

void TA_CityGraph::update(Grid *grid)
{
  for (CentroidGrid::iterator centroid = _gridCentroids.begin(); centroid != _gridCentroids.end(); ++centroid)
  {
    Array<float> energy(size());
    float sum = 0;
    for (size_type i=0; i<size(); ++i)
    {
      energy[i] = 1.0f / distance( *centroid, this->operator[](i) );
      sum += energy[i];
    }
    for (size_type i=0; i<size(); ++i)
      this->operator[](i)->energy += energy[i] / sum;    
  }
}
