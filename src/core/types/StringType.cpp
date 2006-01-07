#include "StringType.h"

#include <QtXml>


const QString StringType::TYPENAME="String";


void StringType::save(QDomDocument &doc, QDomElement &parent) const
{
  QDomAttr valAttr;
  valAttr = doc.createAttribute("Value");
  valAttr.setValue(_value);	
  parent.setAttributeNode(valAttr);
	
}

void StringType::load(QDomElement &typeElem)
{
  _value = typeElem.attribute("Value","");
}
