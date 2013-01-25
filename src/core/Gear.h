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

#include <qobject.h>
#include "Properties.h"
#include "Engine.h"
#include "Project.h"

#include "Timing.h"
#include "Plug.h"
#include "Node.h"
#include <QVector>
#include <QList>
#include <QString>

class QDomDocument;
class QDomElement;
class QGraphicsScene;
class Schema;
class GearMaker;
class BaseGearGui;
class GearInfo;

/**
 * Gear is the atomic processing unit of the dataflow and the base class for all gears.
 * 
 * TODO : setSchema, setName 
 */
class Gear : public QObject, public Node
{
  Q_OBJECT
public:
  static const QString XML_TAGNAME;

  Gear(QString type);
  virtual ~Gear();

  void init();
  void prePlay();
  void postPlay();
  virtual void runAudio(){};
  virtual void runVideo(){};

  void getInputs(QList<AbstractPlug*> &inputs, bool onlyExposed=false) const;
  void getOutputs(QList<AbstractPlug*> &outputs, bool onlyExposed=false) const;
	void getAllPlugs(QList<AbstractPlug*> &plugs) const;

  AbstractPlug* getInput(QString name) const;
  AbstractPlug* getOutput(QString name) const;
	AbstractPlug *getPlug(QString name) const;

  BaseGearGui* getGearGui(){return _gearGui;}
  void setGearGui(BaseGearGui* bggui){_gearGui=bggui;}
  //Node
	void getDependencies(QList<Node*> & dependencies) const;

  virtual Schema* getInternalSchema(){return NULL;}

  void setInstanceName(QString vname);

  QString type();  
  // returns the last type element. Ex : For type Gear:Drone:PushButton, it returns PushButton
  const QString& subType() const {return _subType;};  
  const QString& instanceName() const {return _instanceName;}

  bool ready(){return _ready;}

  void evaluateReady();
  void unSynch();

  //todo make bool
  void save(QDomDocument &, QDomElement &);
  void load(QDomElement &, Drone::LoadingModeFlags lmf, bool loadUUID=true);
 
  virtual bool canConvert(const AbstractType& , const AbstractType& ,
                          std::pair<const AbstractPlug*, const AbstractPlug*>& plugs) const
  {
    plugs.first = plugs.second = 0;
    return false;
  }
	
  bool isPlugNameUnique(QString name);  

  Schema *parentSchema(){return _parentSchema;}
	void parentSchema(Schema &parentSchema);
  GearInfo* getGearInfo(){return _gearInfo;}
  void setGearInfo(GearInfo * gi){_gearInfo=gi;}

  void setUUID(QString uuid){_uuid=uuid;}
  QString getUUID(){return _uuid;}
  

signals:
  void readyStatusChanged();

#ifdef SINGLE_THREADED_PLAYBACK
protected:
#else
public:
#endif

  virtual void internalInit(){}
  virtual void internalSave(QDomDocument&, QDomElement&){}
  virtual void internalLoad(QDomElement &){};
  
  virtual void internalPrePlay(){}  
  virtual void internalPostPlay(){}    

protected:
  virtual void onPlugConnected(AbstractPlug*, AbstractPlug*){};//!connection from one of our plug to other plug
  virtual void onPlugDisconnected(AbstractPlug*, AbstractPlug*){};//!disconnection form one of our plug from other plug
  friend bool AbstractPlug::connect(AbstractPlug *plug);
  friend bool AbstractPlug::disconnect(AbstractPlug *plug);

  
  AbstractPlug* addPlug(AbstractPlug* plug);
  //void addBypassChannel(std::string in, std::string out);
  //TODO: more flexible way of defining this
	void setPlugAtLeastOneNeeded(QList<AbstractPlug*> &plugs);
  //void addPlugAndSubPlugs(AbstractPlug* plug, int level);

  void deletePlug(AbstractPlug *plug);

  Schema *_parentSchema;
  Properties _settings;

  QList<AbstractPlug*> _plugs;

protected:

  QString _subType;
  QString _instanceName;//! unique name of this gear in a schema
  BaseGearGui * _gearGui;
  GearInfo * _gearInfo;
  QString _uuid;

private:

  bool _ready;
	QList<AbstractPlug*> _atLeastOneOfThemNeeded;
  
};

extern "C"
{
  Gear* makeGear();
  //GearInfo getGearInfo();
}


#endif
