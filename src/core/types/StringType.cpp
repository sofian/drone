#include "StringType.h"

#include <qdom.h>


const std::string StringType::TYPENAME="String";


void StringType::save(QDomDocument &doc, QDomElement &parent) const
{
  QDomAttr valAttr;
  valAttr = doc.createAttribute("Value");
  valAttr.setValue(_value.c_str());	
  parent.setAttributeNode(valAttr);
	
}

void StringType::load(QDomElement &typeElem)
{
  _value = typeElem.attribute("Value","").ascii();
}
