#include "Gear.h"

#include "GearGui.h"

#include <qdom.h>


#include <iostream>
#include <sstream>

Gear::Gear(Engine *engine, std::string type, std::string name) : 
_engine(engine), 
_Type(type), 
_Name(name),
_gearGui(NULL)
{
}

Gear::~Gear()
{    
  for (std::list<AbstractPlug*>::iterator it=_Plugs.begin(); it != _Plugs.end(); ++it)
    delete (*it);
}


void Gear::internalPrePlay()
{
  prePlay();
}

void Gear::internalPostPlay()
{
  postPlay();
}

void Gear::internalInit()
{
  std::cout << "__________________________________________" << std::endl;
  std::cout << _Type << std::endl;
  std::cout << "¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯" << std::endl;

  for (std::list<AbstractPlug*>::iterator it=_Plugs.begin(); it != _Plugs.end(); ++it)
    (*it)->init();

  _gearGui = createGearGui(NULL);

  //call the virtual method
  init();
}

//////////////////////////////////////////////////////////////////////
//    PLUGS
//////////////////////////////////////////////////////////////////////

void Gear::deletePlug(AbstractPlug *plug)
{
  delete plug;
  _Plugs.remove(plug);
}


/**
 * hello it's cool
 * 
 * @param name
 * @param default_value
 * 
 * @return 
 */
AbstractPlug* Gear::addPlug(AbstractPlug* plug)
{    
  _Plugs.push_back(plug);

  return plug;    
}

void Gear::getInputs(std::list<AbstractPlug*> &inputs) const
{
  inputs.clear();
  for (std::list<AbstractPlug*>::const_iterator it=_Plugs.begin(); it != _Plugs.end(); ++it)
  {
    if ( ((*it)->inOut() == IN) )
      inputs.push_back(*it);
  }    
}

void Gear::getOutputs(std::list<AbstractPlug*> &outputs) const
{
  outputs.clear();
  for (std::list<AbstractPlug*>::const_iterator it=_Plugs.begin(); it != _Plugs.end(); ++it)
  {
    if ( ((*it)->inOut() == OUT) )
      outputs.push_back(*it);
  }
}

void Gear::getDependencies(std::vector<Gear*> &dependencies) const
{
  std::list<AbstractPlug*> inputs;
  getInputs(inputs);
  std::list<AbstractPlug*> lplug;
  for (std::list<AbstractPlug*>::const_iterator it = inputs.begin();it!=inputs.end();++it)
  {
    (*it)->connectedPlugs(lplug);
    if (lplug.size()!=0 && lplug.back()->parent()->ready())
      dependencies.push_back(lplug.back()->parent());
  }
}

void Gear::needSynch()
{
  _engine->needSynch();
}

GearGui* Gear::createGearGui(QCanvas *canvas)
{
  return new GearGui(this, canvas);    
}


void Gear::internalSave(QDomDocument &doc, QDomElement &parent)
{               
  QDomElement gearElem = doc.createElement("Gear");
  parent.appendChild(gearElem);

  QDomAttr gearType = doc.createAttribute("Type");
  gearType.setValue(_Type.c_str());
  gearElem.setAttributeNode(gearType);

  QDomAttr gearName = doc.createAttribute("Name");
  gearName.setValue(_Name.c_str());
  gearElem.setAttributeNode(gearName);

  _gearGui->save(doc, gearElem);
  _settings.save(doc, gearElem);

  save(doc, gearElem);

}

void Gear::internalLoad(QDomElement &gearElem)               
{            
  _Name = gearElem.attribute("Name","").ascii();

  _settings.load(gearElem);
  updateSettings();

  load(gearElem);

  _gearGui->load(gearElem);

}

AbstractPlug* Gear::getInput(std::string name) const
{
  std::list<AbstractPlug*> inputs;
  getInputs(inputs);
  for (std::list<AbstractPlug*>::const_iterator it = inputs.begin();it!=inputs.end();++it)
  {
    if ((*it)->name() == name)
      return(*it);
  }

  return NULL;
}

AbstractPlug* Gear::getOutput(std::string name) const
{
  std::list<AbstractPlug*> outputs;
  getOutputs(outputs);
  for (std::list<AbstractPlug*>::const_iterator it = outputs.begin();it!=outputs.end();++it)
  {
    if ((*it)->name() == name)
      return(*it);
  }

  return NULL;
}

GearGui* Gear::getGearGui()
{
  return _gearGui;
}

void Gear::updateSettings()
{
  //call the virtual method that can be overriden
  onUpdateSettings();
}
