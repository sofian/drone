/*
 *  Connection.cpp
 *  droneCore
 *
 *  Created by foogy on 20/02/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "Connection.h"

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

void Connection::updateWithRenameMapping(std::map<QString,QString> map)
{
  std::map<QString,QString>::iterator renA(map.find(_gearA));
  std::map<QString,QString>::iterator renB(map.find(_gearB));
  if(renA!=map.end())
    _gearA = map[_gearA];
  if(renB!=map.end())
    _gearB = map[_gearB];
}
