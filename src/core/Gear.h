/* Gear.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef GEAR_INCLUDED
#define GEAR_INCLUDED

#include <string>
#include <list>
#include <vector>

#include "Engine.h"

#include "Properties.h"

#include "Timing.h"
#include "Plug.h"
#include "GearClassification.h"

#include <map>

class GearGui;
class QDomDocument;
class QDomElement;
class QCanvas;
class Schema;


struct GearInfo
{
  GearInfo() : majorVersion(1), minorVersion(1), classification(GearClassifications::unclassified().instance()) {}
  
  std::string name;    
  std::string author;
  std::string description;
  int majorVersion;
  int minorVersion;  
  GearClassification* classification;
};

class Gear  
{
public:
  
  static const std::string XML_TAGNAME;

  //! is this gear a MetaGear? 
  virtual bool isMeta() const {return false;}

  //! is the gear a control gear ?
  virtual bool isControl() const {return false;}

  Gear(Schema *schema, std::string type, std::string uniqueName);
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

  virtual Schema* getInternalSchema(){return NULL;}

  void name(std::string vname){_name=vname;}

  const std::string& type() const {return _Type;};  
  const std::string& name() const {return _name;}
    
  Properties& settings(){return _settings;};

  virtual bool ready()=0;
  void unSynch();
  
  //todo make bool
  virtual void save(QDomDocument &, QDomElement &){};
  virtual void load(QDomElement &){};

  virtual bool canConvert(const AbstractType& , const AbstractType& ,
                          std::pair<const AbstractPlug*, const AbstractPlug*>& plugs) const
  {
    plugs.first = plugs.second = 0;
    return false;
  }

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

  void deletePlug(AbstractPlug *plug);

  Schema *_parentSchema;

  std::list<AbstractPlug*> _plugs;    

  Properties _settings;

private:

  std::string _Type;
  std::string _name;//! unique name of this gear in a schema

  GearGui *_gearGui;

  void internalInit();
  void internalPrePlay();    
  void internalPostPlay();    
  void internalSave(QDomDocument &doc, QDomElement &parent);
  void internalLoad(QDomElement &gearElem);

  friend Gear* Schema::addGear(std::string geartype, std::string uniqueName);
  friend MetaGear* Schema::addMetaGear(std::string name, std::string uniqueName);
  friend void *Engine::playThread(void *parent);
  friend bool Schema::load(QDomElement& parent);
  friend bool Schema::save(QDomDocument& doc, QDomElement &parent);

  #ifdef SINGLE_THREADED_PLAYBACK  
  friend void Engine::debugStartPlay();
  friend void Engine::debugStopPlay();
  #endif

};

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName);
GearInfo getGearInfo();
}


#endif
