#include "GearMaker.h"
#include "GearInfo.h"
#include "Gear.h"
#include <qfile.h>
#include <qdir.h>

const std::string GearInfo_::XML_TAGNAME = "Info";

void GearInfo_::saveDefinition()
{
 std::cout << "void GearInfo_::saveDefinition()"<< std::endl;
   
	GearMaker::saveDefinition(this);
}

bool GearInfo_::createIfNotExists(QString filename, QString fallbackname, GearInfo gi)
{
  QDomDocument doc("GearInfo");
  if(gi.name=="")
    gi.name=fallbackname.latin1();
  QFile file(filename);

  if (!file.open(IO_ReadOnly))
  {
    std::cout << "Failed to open file " << filename << std::endl;
    std::cout << "Creating it ! " << filename << std::endl;

    if (!file.open(IO_ReadWrite))
    {
      std::cout << "-->Failed to create file " << filename << std::endl;
      return false;
    }
    QTextStream stream( &file );
    stream<<"<!DOCTYPE GearInfo>\n<GearInfo>\n  <Info>\n";
    std::string jp;
    std::vector<std::string> path = gi.classification->path();
    for(int i=0;i<path.size();++i)
    {
      jp+=path[i];
      if(i!=path.size()-1)
        jp+="/";
    }
    stream<<"    <Name>"+gi.name+"</Name>\n";
    stream<<"    <Classification>"+jp+"</Classification>\n";
    stream<<"    <Major>0</Major>\n";
    stream<<"    <Minor>1</Minor>\n";
    stream<<"  </Info>\n</GearInfo>";

    file.close();
    file.open(IO_ReadOnly);

  }
}

void GearInfo_::save(QDomDocument &doc, QDomNode &parent)
{
  QDomElement infoElem = doc.createElement(XML_TAGNAME);
  parent.appendChild(infoElem);

  XMLHelper::appendTaggedString(doc,infoElem,"Name",_name);
  XMLHelper::appendTaggedString(doc,infoElem,"Author",_author);
  XMLHelper::appendTaggedString(doc,infoElem,"Minor",QString().arg(_minorVersion));
  XMLHelper::appendTaggedString(doc,infoElem,"Major",QString().arg(_majorVersion));
  XMLHelper::appendTaggedString(doc,infoElem,"Classification",_classification.join("/"));
  XMLHelper::appendTaggedString(doc,infoElem,"Intro",_intro);
  XMLHelper::appendTaggedString(doc,infoElem,"Description",_description);

  QDomElement plugElem = doc.createElement("Plugs");
  infoElem.appendChild(plugElem);

  QMap<QString,PlugInfo>::Iterator it;
  for ( it = _plugs.begin(); it != _plugs.end(); ++it )
  {
std::cerr<<"count : "<<_plugs.size()<<std::endl;
    QValueList<QPair<QString, QString> > vl;
    vl[0]=QPair<QString, QString>("name",it.key());
    vl[0]=QPair<QString, QString>("inout",it.data().inOut==IN?QString("IN"):QString("OUT"));
    XMLHelper::appendTaggedString(doc,infoElem,"Plug",it.data().help,vl);
  }

}


bool GearInfo_::load(QDomNode &parent,QString filename)
{
  QDomNode infoNode = XMLHelper::findChildNode(parent, XML_TAGNAME);
  _filename=filename;

  if (infoNode.isNull())
  {
    std::cout << "Bad Drone GearDefinition file : <"+XML_TAGNAME+"> tag not found in " << _filename<<std::endl;
    return false;
  }

  QDomNode curnode=infoNode.firstChild();
  while(!curnode.isNull())

  {
    QDomElement elem = curnode.toElement();
    //std::cerr<<"found a tag !: "<<elem.tagName().latin1()<<std::endl;

    if(elem.tagName()=="Intro")
      _intro=elem.text();
    else if(elem.tagName()=="Name")
      _name=elem.text();
    else if(elem.tagName()=="Classification")
      _classification =QStringList::split("/",elem.text());
    else if(elem.tagName()=="Plugs")
    {
      QDomNode curnode2=curnode.firstChild();
      while(!curnode2.isNull())
      {
        QDomElement elem2 = curnode2.toElement();
        //std::cerr<<"found a tag !: "<<elem.tagName().latin1()<<std::endl;
        QString name=elem2.attribute("name","?");
        eInOut inout(elem2.attribute("inout","IN")=="IN"?IN:OUT);
        if(elem2.tagName()=="Plug")
        {
          _plugs[name]=PlugInfo();
          _plugs[name].help= elem2.text();
          _plugs[name].type= elem2.attribute("type","?");
          _plugs[name].inOut=inout;
        }
        curnode2=curnode2.nextSibling();
      }
    }

    else if(elem.tagName()=="Description")
      _description = elem.text();
    curnode=curnode.nextSibling();
  }

  return true;

}

void GearInfo_::syncWithGear(Gear*g)
{

  QMap<QString,PlugInfo> pii = g->getInputsInfo();
  QMap<QString,PlugInfo> pio = g->getOutputsInfo();
  PlugInfo pi;
  QMap<QString,PlugInfo>::Iterator it;
  for ( it = pii.begin(); it != pii.end(); ++it )
  {
    pi.help="";
    pi.inOut=IN;
    pi.type=it.data().type;
    _plugs[it.key()]=pi;
  }
  for ( it = pio.begin(); it != pio.end(); ++it )
  {
    pi.help="";
    pi.inOut=OUT;
    pi.type=it.data().type;
    _plugs[it.key()]=pi;
  }

}

void GearInfo_::setPlugInfo(QString name,PlugInfo* pi)
{

  std::cerr<<"GearInfo::setting pluginfo for:"<<name<<" with:"<<pi<<std::endl;
  _plugs[name]=*pi;
}
