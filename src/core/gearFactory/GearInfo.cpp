#include "GearMaker.h"
#include "GearInfo.h"
#include "Gear.h"
#include "GearFrei0r.h"
#include "GearControl.h"

#include <QtXml>
#include <qfile.h>
#include <qdir.h>
#include <QStringList>

#include <dlfcn.h>

const QString GearInfo::XML_TAGNAME = "GearInfo";



GearInfo::GearInfo(QString pluginType) : 
  _type(),
	_pluginType(pluginType), 
	_majorVersion(1), 
	_minorVersion(0),
  _instanciableFromGUI(true)
{
}

bool GearInfo::createDefaultMetaInfo()
{
	//default values
	_classification.push_back("unclassified");
	
	return save();
}


bool GearInfo::save()
{
		
  QDomDocument doc(XML_TAGNAME);
  
  QDomElement infoElem = doc.createElement(XML_TAGNAME);
  doc.appendChild(infoElem);

  XMLHelper::appendTaggedString(doc,infoElem,"Type",_type);// written but never read. Just to make things more obvious when looking at the xml file.
  XMLHelper::appendTaggedString(doc,infoElem,"Author",_author);
  XMLHelper::appendTaggedString(doc,infoElem,"Minor",QString::number(_minorVersion));
  XMLHelper::appendTaggedString(doc,infoElem,"Major",QString::number(_majorVersion));
  XMLHelper::appendTaggedString(doc,infoElem,"Classification",_classification.join("/"));
  XMLHelper::appendTaggedString(doc,infoElem,"Intro",_intro);
  XMLHelper::appendTaggedString(doc,infoElem,"Description",_description);

	QDomElement plugElem = doc.createElement("Plugs");
  infoElem.appendChild(plugElem);

	syncPlugsInfo();
  for (QMap<QString,PlugInfo>::Iterator it = _plugsInfo.begin(); it != _plugsInfo.end(); ++it )
		(*it).save(doc, plugElem);

  QFile file(infoFile().absoluteFilePath());
  if (file.open(QIODevice::WriteOnly))
		file.remove();

  if (file.open(QIODevice::WriteOnly))
  {
    QTextStream stream(&file);
    doc.save(stream,4);
    file.close();
  }
  else
  {
    qCritical() << "file io error, cannot save!";
    return false;
  }
	
	return true;
}

bool GearInfo::load()
{
  // get gear type
  Gear* g = createGearInstance();
  if(!g)
    return false;
  _type = "Gear:"+pluginType() + ":" + g->subType();
    qDebug() << "about to load metainfo for  " << _type;

  delete g;
  
  if(!loadMetaInfo())
  {
    qWarning() << "could not open gear meta Info " << infoFile().fileName() << ", creating default";
		return createDefaultMetaInfo();
  }
	
	return true;
}

bool GearInfo::loadMetaInfo()
{
  qDebug() << "loading: " << infoFile().fileName(); 

	QFile file(infoFile().absoluteFilePath());

	if( !file.open(QIODevice::ReadOnly) )
		return false;

  QDomDocument doc(XML_TAGNAME);
	
  QString errMsg;
  int errLine;
  int errColumn;
  if (!doc.setContent(&file, true, &errMsg, &errLine, &errColumn))
  {
    qCritical() << "gearinfo document is invalid " << file.fileName();
    qCritical() << errMsg;
    qCritical() << "Line: " <<  errLine;
    qCritical() << "Col: " <<  errColumn;
    file.close();
    return false;
  }
  file.close();

  QDomNode infoNode = doc.firstChild();

  if (infoNode.isNull())
  {
    qCritical() << "Bad Drone GearInfo file : <"+XML_TAGNAME+"> tag not found in " << file.fileName();
    return false;
  }

	_plugsInfo.clear();

  QDomNode curnode=infoNode.firstChild();
  while(!curnode.isNull())
  {
    QDomElement elem = curnode.toElement();
    //std::cerr<<"found a  tag !: "<<elem.tagName().latin1()<<std::endl;

    if(elem.tagName()=="Intro")
      _intro=elem.text();
    else if(elem.tagName()=="Classification")
      _classification = elem.text().split("/");
    else if(elem.tagName()=="Plugs")
    {
			QDomNode curnode2=curnode.firstChild();
      while(!curnode2.isNull())
      {
        QDomElement elem2 = curnode2.toElement();
				PlugInfo pi;
				pi.load(elem2);
				addPlugInfo(pi);
        curnode2=curnode2.nextSibling();
      }
			
    }

    else if(elem.tagName()=="Description")
      _description = elem.text();
    curnode=curnode.nextSibling();
  }

	syncPlugsInfo();
  return true;
}

void GearInfo::syncPlugsInfo()
{
  //create a temporary instance of the gear
	Gear *gear = createGearInstance();
	
	if (!gear)
		qCritical() << "cannot sync pluginfo because gear doesnt exists!";

	//remove plugsInfo that doesnt exist anymore
	QList<QString> plugsToRemove;
  for (QMap<QString,PlugInfo>::Iterator it = _plugsInfo.begin(); it != _plugsInfo.end(); ++it )
	{
		if (!gear->getPlug(it.key()))
			plugsToRemove.push_back(it.key());
	}
	foreach(QString plugName, plugsToRemove)
		_plugsInfo.remove(plugName);

	//create plugsInfo that have not been created yet
	QList<AbstractPlug*> plugs; 
	gear->getAllPlugs(plugs);
	foreach(AbstractPlug* plug,plugs)
	{
			if (!_plugsInfo.contains(plug->name()))
			{
				PlugInfo plugInfo(plug->name(), plug->inOut(), plug->abstractDefaultType()->typeName());
				addPlugInfo(plugInfo);
			}
			else
			{
				//sync info that is duplicated in abstractPlug
				PlugInfo plugInfo = getPlugInfo(plug->name());
				plugInfo.inOut(plug->inOut());
				plugInfo.type(plug->abstractDefaultType()->typeName());
				setPlugInfo(plugInfo);
			}	
	}
}

bool GearInfo::addPlugInfo(const PlugInfo& pi)
{
  if (_plugsInfo.contains(pi.name()))
	{
		qDebug() << "addPlugInfo fail because " << pi.name() << " already exists";
		return false;
	}
	
	_plugsInfo.insert(pi.name(), pi);
	
	return true;
}

bool GearInfo::setPlugInfo(const PlugInfo& pi)
{
  if (!_plugsInfo.contains(pi.name()))
	{
		qDebug() << "setPlugInfo fail because " << pi.name() << " doesnt exists";
		return false;
	}
	
	_plugsInfo[pi.name()]=pi;
	
	return true;
}

