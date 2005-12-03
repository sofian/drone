#ifndef SCHEMA_INCLUDED
#define SCHEMA_INCLUDED

#include <qdom.h>
#include <map>
#include <list>
#include <vector>
#include <string>

class MetaGear;
class Gear;
class AbstractPlug;
class Engine; 
class ISchemaEventListener;

class Schema
{
public:

  static const std::string XML_TAGNAME;

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
    void updateWithRenameMapping(std::map<std::string,std::string> map);

  private:
    std::string _gearA;
    std::string _input;
    std::string _gearB;
    std::string _output;
  };


  class ScheduledConnection
  {
  public:
    ScheduledConnection(AbstractPlug *a, AbstractPlug *b) : _a(a), _b(b) {}    
    AbstractPlug *_a;
    AbstractPlug *_b;    
  };


  Schema(MetaGear * parentMetaGear);
  virtual ~Schema();

 
  bool save(QDomDocument& doc, QDomElement &parent, bool onlySelected=false);
  void clear();
  // dx,dy are offsets added to the gears coordinates (used when pasting clipboard)
  bool load(QDomElement& doc, bool pasting=false);

  //! Returns a list of unordered gears, but not expanded. Metagears are left as is.
  virtual std::list<Gear*> getGears(){return _gears;}
  
  //! Returns a list of expanded gears, as GetGears, but where metagears are replaced by their internal schema.
  virtual std::list<Gear*> getDeepGears() const;

  //! Returns a list of ordered gears
  virtual std::list<Gear*> getDeepOrderedReadyGears();

  Gear* getGearByName(std::string name) const;

  //! Returns all connections in this schema
  void getAllConnections(std::list<Connection*> &connections);


  bool connect(AbstractPlug *plugA, AbstractPlug *plugB);
  bool connect(Schema::Connection &connection);
  void disconnect(AbstractPlug *plugA, AbstractPlug *plugB);
  void disconnectAll(AbstractPlug *plugA);

  MetaGear* newMetaGear();
  MetaGear* addMetaGear(std::string filename);
  void renameGear(Gear* gear, std::string newName);
  Gear* addGear(std::string geartype);
  
  bool removeDeepGear(Gear* gear);
  
  std::string getUniqueGearName(std::string prefix);
  std::list<Schema*> getSubSchemas();
  
  void unSynch(){_needSynch=true;}  

  
  void lock(){_locked=true;}
  void unlock();
    
  void initGear(Gear * gear) const;

  void addSchemaEventListener(ISchemaEventListener *schemaEventListener);
  void removeSchemaEventListener(ISchemaEventListener *schemaEventListener);

  Schema *getParentSchema();

 private:

  Gear* addGear(std::string geartype, std::string uniqueName);
  MetaGear* addMetaGear(std::string name, std::string uniqueName);

  void onGearAdded(Gear *gear);
  void onGearRemoved(Gear *gear);


  bool needSynch();
  void synch();

  bool _needSynch;

  std::list<Gear*> _gears;

  std::list<Gear*> _lastDeepOrderedReadyGears;
  
  MetaGear *_parentMetaGear;

  //thread safety
  bool _locked;
  std::vector<ScheduledConnection> _scheduledConnections;
  std::vector<ScheduledConnection> _scheduledDisconnections;
  std::vector<Gear*> _scheduledDeletes;
  std::list<ISchemaEventListener*> _schemaEventListeners;

    
};

#endif
