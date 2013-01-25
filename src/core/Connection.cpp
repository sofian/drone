/*
 *  Connection.cpp
 *  droneCore
 *
 *  Created by foogy on 20/02/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "Connection.h"

// c.f. http://harmattan-dev.nokia.com/docs/library/html/qt4/debug.html#providing-support-for-the-qdebug-stream-operator
QDebug	operator<< ( QDebug out, const Connection & conn )
{

  out<<"Connection from :"<<conn.gearA()<<"("<<conn.input()<<") to "<<conn.gearB()<<"("<<conn.output()<<")";
  return out.space();
}
 

void Connection::save(QDomDocument &doc, QDomElement &parent)
{
  QDomElement connectionElem = doc.createElement("Connection");
  parent.appendChild(connectionElem);

  QDomAttr gearA = doc.createAttribute("GearA");
  gearA.setValue(_gearA);
  connectionElem.setAttributeNode(gearA);

  QDomAttr input = doc.createAttribute("Input");
  input.setValue(_input);
  connectionElem.setAttributeNode(input);

  QDomAttr gearB = doc.createAttribute("GearB");
  gearB.setValue(_gearB);
  connectionElem.setAttributeNode(gearB);

  QDomAttr output = doc.createAttribute("Output");
  output.setValue(_output);
  connectionElem.setAttributeNode(output);

}

void Connection::load(QDomElement &connectionElem)
{
  _gearA = connectionElem.attribute("GearA","");
  _input = connectionElem.attribute("Input","");
  _gearB = connectionElem.attribute("GearB","");
  _output = connectionElem.attribute("Output","");
}

void Connection::updateWithRenameMapping(QMap<QString,QString> map)
{
  QString renA(map[_gearA]);
  QString renB(map[_gearB]);
  if(!renA.isEmpty())
    _gearA = map[_gearA];
  if(!renB.isEmpty())
    _gearB = map[_gearB];
}
