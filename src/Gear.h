#ifndef GEAR_INCLUDED
#define GEAR_INCLUDED

#include <string>
#include <list>
#include <vector>

#include "Engine.h"

#include "Properties.h"

#include "Timing.h"
#include "Plug.h"


class GearGui;
class QDomDocument;
class QDomElement;
class QCanvas;


// struct AutoDuplicatePlugGroup
// {
//   int nbMin, nbMax;
//   std::vector<AbstractPlug* > plugs;
// };

class Gear  
{
public:


  Gear(Engine* engine, std::string type, std::string name);
  virtual ~Gear();

  virtual void init(){};
  virtual void prePlay(){};    
  virtual void postPlay(){};    
  virtual void runAudio(){};//! test pour ready doit etre fait avant
  virtual void runVideo(){};//! test pour ready doit etre fait avant
  GearGui* getGearGui();

  void updateSettings();//! user must call this method after settings have been modified

  void getInputs(std::list<AbstractPlug*> &inputs) const;
  void getOutputs(std::list<AbstractPlug*> &outputs) const;

  AbstractPlug* getInput(std::string name) const;
  AbstractPlug* getOutput(std::string name) const;

  void getDependencies(std::vector<Gear*> & dependencies) const;

  const std::string& type() const {return _Type;};
  const std::string& name() const {return _Name;};

  Properties& settings(){return _settings;};

  virtual bool ready()=0;
  void needSynch();

  virtual void save(QDomDocument &, QDomElement &){};
  virtual void load(QDomElement &){};


  Engine *engine(){return _engine;};

protected:

  //! overload to create your own GearGui
  virtual GearGui* createGearGui(QCanvas *canvas);

  virtual void onUpdateSettings(){};
  virtual void onPlugConnected(AbstractPlug*){};
  virtual void onPlugDisconnected(AbstractPlug*){}; 
  friend bool AbstractPlug::connect(AbstractPlug *plug);
  friend bool AbstractPlug::disconnect(AbstractPlug *plug);

  AbstractPlug* addPlug(AbstractPlug* plug);       
  void addPlugAndSubPlugs(AbstractPlug* plug, int level);

//   template<class T>
//   void addAutoDuplicatePlugGroup<T>(Abstract>* plug, std::vector< Plug<T>* > zeVector, int nbMin, int nbMax);
//     {
//       zeVector.push_back(plug);
//       _autoDuplicatePlugsVectors.push_back(zeVector);
//     }

  void deletePlug(AbstractPlug *plug);

  Engine *_engine;

  std::list<AbstractPlug*> _Plugs;    

  Properties _settings;

private:

//   std::vector< AutoDuplicateGroup > _autoDuplicatePlugsVectors;

  std::string _Type;
  std::string _Name;

  GearGui *_gearGui;

  void internalInit();
  void internalPrePlay();    
  void internalPostPlay();    
  void internalSave(QDomDocument &doc, QDomElement &parent);
  void internalLoad(QDomElement &gearElem);

  friend Gear* Engine::addGear(std::string geartype, std::string name);
  friend void *Engine::playThread(void *parent);
  friend void Engine::loadSchema(std::string filename);
  friend void Engine::saveSchema(std::string filename);
};




#endif
