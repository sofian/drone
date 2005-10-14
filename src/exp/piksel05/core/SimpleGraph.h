#ifndef SIMPLEGRAPH_INCLUDED
#define SIMPLEGRAPH_INCLUDED

#include "error.h"
#include <map>
#include <set>
#include <string>


template <class T>
class SimpleGraph : public std::map<std::string, T>
{
  typedef std::set<std::pair<std::string, std::string> > SimpleEdges;
  typedef std::map<std::string, T> parent;
  
public:
//   //! Inner type definitions.
//   typedef typename std::map<string, T>::size_type size_type;
//   typedef typename std::map<string, T>::iterator iterator;
//   typedef typename std::map<string, T>::const_iterator const_iterator;
//   typedef typename std::map<string, T>::reference reference;
//   typedef typename std::map<string, T>::const_reference const_reference;
//   typedef typename std::map<string, T>::pointer pointer;

  
// public:
//   // using (as suggested by Norm)
//   using std::map<string, T>::end;
//   using std::map<string, T>::begin;
//   using std::map<string, T>::front;

  SimpleGraph() : parent() {}
  
  virtual ~SimpleGraph() {}

  void clear()
  {
    parent::clear();
    _edges.clear();
  }

  void addEdge(const std::string& from, const std::string& to, bool twoWays = true)
  {
    // Check that they exist.
    ASSERT_ERROR( find(from) != end() );
    ASSERT_ERROR( find(to) != end() );

    // Add it.
    _edges.insert(std::make_pair(from, to));
    
    if (twoWays)
      _edges.insert(std::make_pair(to, from));
  }
  
  bool hasEdge(const std::string& from, const std::string& to)
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
