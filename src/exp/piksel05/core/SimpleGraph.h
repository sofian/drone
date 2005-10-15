#ifndef SIMPLEGRAPH_INCLUDED
#define SIMPLEGRAPH_INCLUDED

#include "error.h"
#include <map>
#include <set>
#include <string>

template <class K, class T>
class SimpleGraph : public std::map<K, T>
{
  typedef std::set<std::pair<K, K> > SimpleEdges;
  typedef std::map<K, T> parent;
  
public:
  SimpleGraph() : parent() {}
  
  ~SimpleGraph() {}

  void clear()
  {
    parent::clear();
    _edges.clear();
  }

  void addEdge(const K& from, const K& to, bool twoWays = true)
  {
    // Check that they exist.
    ASSERT_ERROR( find(from) != end() );
    ASSERT_ERROR( find(to) != end() );

    // Add it.
    _edges.insert(std::make_pair(from, to));
    
    if (twoWays)
      _edges.insert(std::make_pair(to, from));
  }
  
  bool hasEdge(const K& from, const K& to)
  {
    // Check that they exist.
    ASSERT_ERROR( find(from) != end() );
    ASSERT_ERROR( find(to) != end() );

    return (_edges.find(std::make_pair(from, to)) != _edges.end());
  }
  
protected:
  SimpleEdges _edges;
};

#endif
