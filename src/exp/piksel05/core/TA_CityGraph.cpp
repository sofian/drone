#include "TA_CityGraph.h"
#include <iostream>

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

float distance(TA_CityVertex *a, TA_CityVertex *b)
{
  float xdiff = (a->x - b->x);
  float ydiff = (a->y - b->y);
  return sqrt( xdiff*xdiff + ydiff*ydiff );
}

// TA_CityGraph::TA_CityGraph(size_type nHotSpots, size_type nCentroids)
//  : SimpleGraph<TA_CityVertex*>(nHotSpots),
//    _gridCentroids(nCentroids,nCentroids)
// {
  
// }

TA_CityGraph::TA_CityGraph(const std::string& filename)
{
  load(filename);
}

TA_CityGraph::~TA_CityGraph()
{
  // Clear up.
  for (iterator it = begin(); it != end(); ++it)
    delete it->second;
}

void TA_CityGraph::update(TA_Grid *grid)
{
  for (TA_CentroidGrid::iterator centroid = _gridCentroids.begin(); centroid != _gridCentroids.end(); ++centroid)
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

#include <iostream>
void TA_CityGraph::load(const std::string& filename)
{
  std::cout << "loading file : " << filename << std::endl;
  QDomDocument doc;
  QFile file(filename);
  if (!file.open(IO_ReadOnly))
    return;
  if (!doc.setContent(&file))
  {
    file.close();
    return;
  }
  file.close();

  // print out the element names of all elements that are direct children
  // of the outermost element.
  QDomElement docElem = doc.documentElement();

  QDomNode n = docElem.firstChild();
  while(!n.isNull())
  {
    QDomElement e = n.toElement(); // try to convert the node to an element.
    if(!e.isNull())
    {
      if (e.tagName() == "spot")
      {
        TA_CityVertex *vertex = new TA_CityVertex;
        std::string id = e.attribute("id");
        ASSERT_WARNING( id != "" );
        //        insert(make_pair(, vertex));
        QDomNode m = e.firstChild();
        while (!m.isNull())
        {
          QDomElement f = m.toElement();
          std::cout << f.tagName() << std::endl;
          m = m.nextSibling();
        }
        
      }
      else if (e.tagName() == "connection")
      {
        
      }
    }
    n = n.nextSibling();
  }

  // Here we append a new element to the end of the document
  QDomElement elem = doc.createElement("img");
  elem.setAttribute("src", "myimage.png");
  docElem.appendChild(elem);  
}
