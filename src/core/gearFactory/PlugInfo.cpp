/* 
 * File:   PlugInfo.cpp
 * Author: julien
 * 
 * Created on January 20, 2013, 8:35 PM
 */

#include <QtXml>

#include "PlugInfo.h"
#include "XMLHelper.h"
#include "Project.h"

bool PlugInfo::save(QDomDocument doc, QDomElement parent) const
{
	QList<QPair<QString, QString> > vl;
	vl.push_back(QPair<QString, QString>("name",_name));
	vl.push_back(QPair<QString, QString>("type",_type));
	vl.push_back(QPair<QString, QString>("inout",_inOut==IN?QString("IN"):QString("OUT")));
	XMLHelper::appendTaggedString(doc, parent, "Plug", _help, vl);
	
	return true;
}

bool PlugInfo::load(QDomElement elem)
{
	//std::cerr<<"found a tag !: "<<elem.tagName().latin1()<<std::endl;
	_name=elem.attribute("name","?");
	_inOut=elem.attribute("inout","IN")=="IN"?IN:OUT;
	_type = elem.attribute("type","?");
	_help = elem.text();
	
	return true;
}
