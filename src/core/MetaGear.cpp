#include "MetaGear.h"
#include "GearInfo.h"
#include "XMLHelper.h"

#include <qfileinfo.h>

const QColor MetaGear::METAGEAR_COLOR(115,8,8);
const QString MetaGear::TYPE="MetaGear";
const QString MetaGear::EXTENSION=".meta";

MetaGear::MetaGear() :
Gear(TYPE),
_metaGearName(TYPE)
{
  _schema = new Schema(*this);
}

MetaGear::~MetaGear()
{
  //TODOFOO: check this!!?
  //_schema.removeAllGears();
  //_schema->removeSchemaEventListener(this);
}

void MetaGear::internalSave(QDomDocument &doc, QDomElement &parent)
{
  _schema->save(doc, parent);
}

void MetaGear::internalLoad(QDomElement &parent)
{

  //find the schema node under the metagear
  QDomNode schemaNode = XMLHelper::findChildNode(parent, Schema::XML_TAGNAME);

  if (schemaNode.isNull())
  {
    std::cout << "Bad DroneSchema : <Schema> tag not found for metagear!" << std::endl;
    return;
  }

  QDomElement schemaElem = schemaNode.toElement();
  _schema->load(schemaElem);

  createPlugs();
}

void MetaGear::createPlugs()
{
  //clear plugs first, we will recreate them
  qDeleteAll(_plugs);

  //clear plug mapping
  _plugMapping.clear();

  QList<Gear*> gears = _schema->getGears();

  QList<AbstractPlug*> inputs;
  QList<AbstractPlug*> outputs;

  for(QList<Gear*>::iterator gearIt=gears.begin();gearIt!=gears.end();++gearIt)
  {
    (*gearIt)->getInputs(inputs, true);
    for(QList<AbstractPlug*>::Iterator plugIt=inputs.begin(); plugIt!=inputs.end(); ++plugIt)
    {
      AbstractPlug* clone = (*plugIt)->clone(this);
      clone->name((*plugIt)->fullName());
      clone->forwardPlug(*plugIt);
      addPlug(clone);
//       _plugMapping[clone]=*plugIt;
    }

    (*gearIt)->getOutputs(outputs, true);
    for(QList<AbstractPlug*>::Iterator plugIt=outputs.begin(); plugIt!=outputs.end(); ++plugIt)
    {
      AbstractPlug* clone = (*plugIt)->clone(this);
      clone->name((*plugIt)->fullName());
      clone->forwardPlug(*plugIt);
      addPlug(clone);
//       _plugMapping[clone]=*plugIt;
    }
  }

}

void MetaGear::save(QString filename)
{  
  QDomDocument doc("MetaGear");
    
  QDomElement metaGearElem = doc.createElement("MetaGear");
  doc.appendChild(metaGearElem);
    
  Gear::save(doc, metaGearElem);

  QFile file(filename);
  if (file.open(QIODevice::WriteOnly))
  {
    QTextStream stream(&file);
    doc.save(stream,4);
    file.close();
  }
  else
  {
    std::cout << "file io error, cannot save!" << std::endl;
    return;
  }

  _fullPath = filename;
}

bool MetaGear::load(QString filename)
{
  QDomDocument doc("MetaGear");

  QFile file(filename);

  if (!file.open(QIODevice::ReadOnly))
  {
    qCritical() << "Fail to open file: " << filename;
    return false;
  }

  QString errMsg;
  int errLine;
  int errColumn;
  if (!doc.setContent(&file, true, &errMsg, &errLine, &errColumn))
  {
    qCritical() << "parsing error in " << filename;
    qCritical() << errMsg;
    qCritical() << "Line: " <<  errLine;
    qCritical() << "Col: " <<  errColumn;
    file.close();
    return false;
  }

  file.close();

  QDomNode metagearNode = doc.firstChild();
  QDomElement metagearElem = metagearNode.toElement();
  
  if (metagearElem.isNull())
  {
    std::cout << "Bad drone metagear, metagear elem isNull" << std::endl;
    return false;
  }

  //save the path
  _fullPath = filename;
  
  //load
  Gear::load(metagearElem);

  return true;
}


