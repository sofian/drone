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

#include <map>

class GearGui;
class QDomDocument;
class QDomElement;
class QCanvas;
class Schema;

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

  virtual Schema* getInternalSchema(){return NULL;}


  const std::string& type() const {return _Type;};
  const std::string& name() const {return _Name;};

  Properties& settings(){return _settings;};

  virtual bool ready()=0;
  void needSynch();

  virtual void save(QDomDocument &, QDomElement &){};
  virtual void load(QDomElement &){};

  virtual bool canConvert(const AbstractType& , const AbstractType& ,
                          std::pair<const AbstractPlug*, const AbstractPlug*>& plugs) const
  {
    plugs.first = plugs.second = 0;
    return false;
  }

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

  void deletePlug(AbstractPlug *plug);

  Engine *_engine;

  std::list<AbstractPlug*> _plugs;    

  Properties _settings;

private:

  std::string _Type;
  std::string _Name;

  GearGui *_gearGui;

  void internalInit();
  void internalPrePlay();    
  void internalPostPlay();    
  void internalSave(QDomDocument &doc, QDomElement &parent);
  void internalLoad(QDomElement &gearElem);

  friend Gear* Schema::addGear(Engine * engine, std::string geartype, std::string name);
  friend void *Engine::playThread(void *parent);
  friend void Schema::load(Engine * engine, std::string filename);
  friend void Schema::save(std::string filename);
};




#endif
