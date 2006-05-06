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

#include "Engine.h"

#include "Timing.h"
#include "Plug.h"
#include "GearClassification.h"
#include "GearInfo.h"
#include "Node.h"
#include <QVector>
#include <QList>
#include <QString>

class QDomDocument;
class QDomElement;
class QCanvas;
class Schema;
class GearMaker;

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

  //Node
	void getDependencies(QList<Node*> & dependencies) const;

  virtual Schema* getInternalSchema(){return NULL;}

  void name(QString vname);

  const QString& type() const {return _Type;};  
  const QString& name() const {return _name;}

  bool ready(){return _ready;}

  void evaluateReady();
  void unSynch();

  //todo make bool
  void save(QDomDocument &, QDomElement &);
  void load(QDomElement &);
 
  virtual bool canConvert(const AbstractType& , const AbstractType& ,
                          std::pair<const AbstractPlug*, const AbstractPlug*>& plugs) const
  {
    plugs.first = plugs.second = 0;
    return false;
  }
	
  bool isPlugNameUnique(QString name);  

  Schema *parentSchema(){return _parentSchema;}
  //GearInfo* getGearInfo_(){return _gearInfo;}

signals:
  void readyStatusChanged();

protected:

  virtual void internalInit(){}
  virtual void internalSave(QDomDocument&, QDomElement&){}
  virtual void internalLoad(QDomElement &){};
  
  virtual void internalPrePlay(){}  
  virtual void internalPostPlay(){}    
    
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

  QList<AbstractPlug*> _plugs;

protected:

  QString _Type;
  QString _name;//! unique name of this gear in a schema

  //GearInfo * _gearInfo;

private:

  bool _ready;
	QList<AbstractPlug*> _atLeastOneOfThemNeeded;
  
};

extern "C"
{
  Gear* makeGear(Schema *schema, QString uniqueName);
  //GearInfo getGearInfo();
}


#endif
