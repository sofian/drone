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
#include <qobject.h>

#include "Engine.h"

#include "Properties.h"

#include "Timing.h"
#include "Plug.h"
#include "GearClassification.h"
#include "GearInfo.h"
#include <qvector.h>
#include <qvaluelist.h>
#include <qstring.h>
#include <map>


class GearGui;
class QDomDocument;
class QDomElement;
class QCanvas;
class Schema;
class GearInfo_;
class GearMaker;

#include "oldGearInfo.h"

/**
 * Gear is the atomic processing unit of the dataflow and the base class for all gears.
 * 
 * 
 */
class Gear : public QObject
{
  Q_OBJECT
public:

  enum GearKind {GEAR, METAGEAR, CONTROL, EXPOSE_ANCHOR};

  static const std::string XML_TAGNAME;

  virtual GearKind kind() const {return GEAR;}

  Gear(Schema *parentSchema, std::string type, std::string uniqueName);
  virtual ~Gear();

  void init();
  void prePlay();
  void postPlay();
  virtual void runAudio(){};
  virtual void runVideo(){};
  GearGui* getGearGui();

  void updateSettings();

  void getInputs(std::list<AbstractPlug*> & inputs, bool onlyExposed=false) const;
  void getOutputs(std::list<AbstractPlug*> &outputs, bool onlyExposed=false) const;

  QMap<QString,PlugInfo>  getInputsInfo() const;
  QMap<QString,PlugInfo>  getOutputsInfo() const;


  AbstractPlug* getInput(std::string name) const;
  AbstractPlug* getOutput(std::string name) const;

  void getDependencies(std::vector<Gear*> & dependencies) const;

  virtual Schema* getInternalSchema(){return NULL;}

  void name(std::string vname){_name=vname;}

  const std::string& type() const {return _Type;};
  const std::string& name() const {return _name;}

  Properties& settings(){return _settings;};

  bool ready(){return _ready;}
  void evaluateReady();
  void unSynch();

  //todo make bool
  void save(QDomDocument &, QDomElement &);
  void load(QDomElement &);
 
  void saveDefinition(QDomDocument& doc);


  virtual bool canConvert(const AbstractType& , const AbstractType& ,
                          std::pair<const AbstractPlug*, const AbstractPlug*>& plugs) const
  {
    plugs.first = plugs.second = 0;
    return false;
  }


  bool isPlugNameUnique(std::string name);

  Schema *parentSchema(){return _parentSchema;}
  GearInfo_* getGearInfo_(){return _gearInfo_;}
signals:
  void readyStatusChanged();

protected:

  virtual void internalInit(){}
  virtual void internalSave(QDomDocument&, QDomElement&){}
  virtual void internalLoad(QDomElement &){};

  virtual void internalPrePlay(){}
  virtual void internalPostPlay(){}

  //! overload to create your own GearGui
  virtual GearGui* createGearGui(QCanvas *canvas);

  virtual void onUpdateSettings(){};
  virtual void onPlugConnected(AbstractPlug*, AbstractPlug*){};//!connection from one of our plug to other plug
  virtual void onPlugDisconnected(AbstractPlug*, AbstractPlug*){};//!disconnection form one of our plug from other plug
  friend bool AbstractPlug::connect(AbstractPlug *plug);
  friend bool AbstractPlug::disconnect(AbstractPlug *plug);

  AbstractPlug* addPlug(AbstractPlug* plug);
  //void addBypassChannel(std::string in, std::string out);
  void setPlugAtLeastOneNeeded(std::vector<AbstractPlug*> &plugs);
  //void addPlugAndSubPlugs(AbstractPlug* plug, int level);

  void deletePlug(AbstractPlug *plug);

  Schema *_parentSchema;

  std::list<AbstractPlug*> _plugs;

  Properties _settings;

protected:

  std::string _Type;
  std::string _name;//! unique name of this gear in a schema

  GearGui *_gearGui;
  GearInfo_ * _gearInfo_;
private:

  bool _ready;
  std::vector<AbstractPlug*> _atLeastOneOfThemNeeded;

  friend class GearMaker;
  
  friend void Schema::initGear(Gear* gear) const;
  friend void *Engine::playThread(void *parent);
  friend bool Schema::load(QDomElement& parent, bool pasting);
  friend bool Schema::save(QDomDocument& doc, QDomElement &parent, bool onlySelected);

#ifdef SINGLE_THREADED_PLAYBACK
  friend void Engine::debugStartPlay();
  friend void Engine::debugStopPlay();
#endif

};

extern "C"
{
  Gear* makeGear(Schema *schema, std::string uniqueName);
  GearInfo getGearInfo();
}


#endif
