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
const QString GearInfoControl::TYPENAME = "Control";
const QString GearInfoDrone::TYPENAME = "Drone";
const QString GearInfoFrei0r::TYPENAME = "Frei0r";
const QString GearInfoMeta::TYPENAME = "Meta";


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

GearInfo::GearInfo(QString pluginType, QFileInfo pluginFile) : 
	_pluginType(pluginType),
	_pluginFile(pluginFile), 
	_majorVersion(1), 
	_minorVersion(0)
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
	if (!_pluginFile.exists())
		return false;
		
  QDomDocument doc(XML_TAGNAME);
  
  QDomElement infoElem = doc.createElement(XML_TAGNAME);
  doc.appendChild(infoElem);

  XMLHelper::appendTaggedString(doc,infoElem,"Name",type());// written but never read. Just to make things more obvious when looking at the xml file.
  XMLHelper::appendTaggedString(doc,infoElem,"Type",pluginType());// written but never read. Just to make things more obvious when looking at the xml file.
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


QString GearInfo::type()
{
  QString name = infoFile().baseName();
  return name;
}

bool GearInfo::load()
{
	if (!_pluginFile.exists())
	{
		qCritical() << "plugin file doesnt exists: " << _pluginFile.fileName(); 
		return false;
	}
	
	if (!bindPlugin())
		return false;

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
    std::cerr<<"found a  tag !: "<<elem.tagName().latin1()<<std::endl;

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

/**
* GearInfo for drone gears.
**/
GearInfoDrone::GearInfoDrone(QFileInfo pluginFile) :
	GearInfo(TYPENAME, pluginFile),
	_handle(0),
	_makeGear(0)
{
  qDebug();
}

GearInfoDrone::~GearInfoDrone()
{
	if (_handle)
		dlclose(_handle);
}

bool GearInfoDrone::bindPlugin()
{
	qDebug() << "binding: " << _pluginFile.fileName();

	//open file
	_handle = dlopen(_pluginFile.absoluteFilePath().toAscii(), RTLD_LAZY);

	if (!_handle)
	{
		qCritical() << _pluginFile.absoluteFilePath() << " : " << dlerror();
		return false;
	}
	
	//query makeGear ptrfun interface
	Gear* (*makeGear)();
	*(void**) (&_makeGear) = dlsym(_handle, "makeGear");	
	char*e=dlerror();
	if (e)
	{
		qCritical() << _pluginFile.absoluteFilePath() << " : not a drone plugin!. Error: '"<<e<<"'";
		return false;
	}

  
	return true;
}

Gear* GearInfoDrone::createGearInstance()
{
	if (_makeGear)
		return _makeGear();
	else
		return NULL;
}


GearInfoControl::GearInfoControl(QFileInfo pluginFile) :
	GearInfo(TYPENAME, pluginFile),
	_handle(0)
{
}

GearInfoControl::~GearInfoControl()
{
	if (_handle)
		dlclose(_handle);
}

bool GearInfoControl::bindPlugin()
{
 	qDebug() << "binding: " << _pluginFile.fileName();

	//open file
	_handle = dlopen(_pluginFile.absoluteFilePath().toAscii(), RTLD_LAZY);

	if (!_handle)
	{
		qCritical() << _pluginFile.absoluteFilePath() << " : " << dlerror();
		return false;
	}
	
	//query makeControl ptrfun interface
	Control* (*makeControl)();
	*(void**) (&_makeControl) = dlsym(_handle, "makeControl");	
	char*e=dlerror();
	if (e)
	{
		qCritical() << _pluginFile.absoluteFilePath() << " : not a drone control!. Error: '"<<e<<"'";
		return false;
	}

 	return true;
}

Gear* GearInfoControl::createGearInstance()
{
  Control* control;
  if (_makeControl)
    control = _makeControl();
  else
    return NULL;

  GearControl* gear = (GearControl*)GearMaker::instance()->makeGear("Gear:Drone:" + control->getGearType());
  if(!gear)
    return;
  gear->setControl(control);

}

GearInfoFrei0r::GearInfoFrei0r(QFileInfo pluginFile) :
	GearInfo(TYPENAME, pluginFile),
	_handle(0)
{
}

GearInfoFrei0r::~GearInfoFrei0r()
{
	if (_handle)
		dlclose(_handle);
}

bool GearInfoFrei0r::bindPlugin()
{
  qDebug() << "binding: " << _pluginFile.fileName();
	
	_handle = dlopen(_pluginFile.absoluteFilePath().toAscii(), RTLD_LAZY);

	if (!_handle)
	{
		qCritical() << _pluginFile.absoluteFilePath() << " : " << dlerror();
		return false;
	}
	
  void (*f0r_get_plugin_info)(f0r_plugin_info_t*);
  *(void**) (&f0r_get_plugin_info) = dlsym(_handle, "f0r_get_plugin_info");
	
	if (dlerror())
	{
		qCritical() << _pluginFile.absoluteFilePath() << " : not a Frei0r plugin!";
		return false;
	}
	
  (*f0r_get_plugin_info)(&_pluginInfo);

	_majorVersion = _pluginInfo.major_version;
  _minorVersion = _pluginInfo.minor_version;
	_author = QString::fromLocal8Bit(_pluginInfo.author, strlen(_pluginInfo.author)) ;
	
	return true;
}

bool GearInfoFrei0r::loadMetaInfo()
{
	if (!GearInfo::loadMetaInfo())
		return false;
	
	//overwrite field already specified in f0r_plugin_info_t
	_majorVersion = _pluginInfo.major_version;
  _minorVersion = _pluginInfo.minor_version;
	_author = QString::fromLocal8Bit(_pluginInfo.author, strlen(_pluginInfo.author)) ;
	
	return true;
}

Gear* GearInfoFrei0r::createGearInstance()
{
  return new GearFrei0r(_handle);
}


GearInfoMeta::GearInfoMeta(QFileInfo pluginFile) :
	GearInfo(TYPENAME, pluginFile)
{
}

GearInfoMeta::~GearInfoMeta()
{
}

Gear* GearInfoMeta::createGearInstance()
{
	return NULL;
}
	
bool GearInfoMeta::bindPlugin()
{
	return true;
}
