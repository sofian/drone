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
  //! Inner type definitions.
  typedef typename parent::size_type size_type;
  typedef typename parent::iterator iterator;
  typedef typename parent::const_iterator const_iterator;
  typedef typename parent::reference reference;
  typedef typename parent::const_reference const_reference;
  typedef typename parent::pointer pointer;

public:
  // using (as suggested by Norm)
  using parent::end;
  using parent::begin;

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
  
  bool hasEdge(const K& from, const K& to) const
  {
    // Check that they exist.
    ASSERT_ERROR( find(from) != end() );
    ASSERT_ERROR( find(to) != end() );

    return (_edges.find(std::make_pair(from, to)) != _edges.end());
  }

  std::set<K> neighbors(const K& e) const
  {
    std::set<K> neighbors;
    for (const_iterator it = begin(); it != end(); ++it)
      if (hasEdge(e, it->first))
        neighbors.insert(it->first);
    return neighbors;
  }
  
protected:
  SimpleEdges _edges;
};

#endif
