#ifndef SCHEMA_INCLUDED
#define SCHEMA_INCLUDED

#include <qdom.h>
#include <list>
#include <vector>
#include <string>

class MetaGear;
class Gear;
class AbstractPlug;
class Engine; 

class Schema
{
public:

  class GearGraphManip
  {
  public :
    class Node
    {
    public:
      Node(Gear* pgear) :
        gear(pgear),
        order(0),
        visited(false){}

      Gear* gear;
      int order;
      bool visited;
    };

    GearGraphManip(std::vector<Gear*> &gears);    
    void getOrderedGears(std::list<Gear*>& orderedGears);
    void labelling(Node &node);

  protected:
    static bool compareNodes(const Node &a, const Node &b);

    std::vector<Node> _nodes;
    std::vector<Gear*> _gears;
    int _depthFirstCounter;
  };

  
  class Connection
  {
  public:
    Connection(){};        
    Connection(std::string gearA, std::string output, std::string gearB, std::string input) :
      _gearA(gearA),
      _input(input),
      _gearB(gearB),
      _output(output)
    {
    }

    std::string gearA(){return _gearA;};
    std::string input(){return _input;};
    std::string gearB(){return _gearB;};
    std::string output(){return _output;};

    void save(QDomDocument &doc, QDomElement &parent);
    void load(QDomElement &connectionElem);

  private:
    std::string _gearA;
    std::string _input;
    std::string _gearB;
    std::string _output;
  };

  Schema(MetaGear * parentMetaGear=NULL);
  virtual ~Schema();

  void save(std::string filename);
  void clear();
  void load(Engine * engine, std::string filename);

  //! Returns a list of unordered gears, but not expanded. Metagears are left as is.
  virtual std::list<Gear*> getGears(){return _gears;}
  
  //! Returns a list of expanded gears, as GetGears, but where metagears are replaced by their internal schema.
  virtual std::list<Gear*> getDeepGears() const;

  //! Returns a list of ordered gears
  virtual std::list<Gear*> getDeepOrderedReadyGears();

  //! return a gear by its name
  Gear* getDeepGearByName(std::string name) const;

  Gear* getGearByName(std::string name) const;

  //! Returns all connections in this schema
  void getAllConnections(std::list<Connection*> &connections);


  Gear* addMetaGear(Engine * engine, std::string geartype, std::string name);
  Gear* addGear(Engine * engine, std::string geartype, std::string name);

  //! removes the specified gear, searching recursivly in sub-schemas
  bool removeDeepGear(Gear* gear);
  
  std::string getNewGearName(std::string prefix);
  std::list<Schema*> getSubSchemas();
  
  void connectPlugs(Schema::Connection &connection);

private:

  bool needSynch();
  void setSynched();

  bool _needSynch;


  std::list<Gear*> _gears;

  std::list<Gear*> _lastDeepOrderedReadyGears;
  
  MetaGear *_parentMetaGear;
  
  static Schema _mainSchema;

};

#endif
