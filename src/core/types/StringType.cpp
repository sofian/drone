#include "StringType.h"

#include <QtXml>


void StringType::save(QDomDocument &doc, QDomElement &parent) const
{
  QDomAttr valAttr;
  valAttr = doc.createAttribute("Value");
  valAttr.setValue(_value);	
  parent.setAttributeNode(valAttr);
	
}

void StringType::load(QDomElement &typeElem, Drone::LoadingModeFlags lmf)
{
  Q_UNUSED(lmf);
  _value = typeElem.attribute("Value","");
}
