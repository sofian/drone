#include "MetaGear.h"
#include "GearGui.h"

#include "XMLHelper.h"
#include "ControlPanel.h"

#include <qfileinfo.h>

const QColor MetaGear::METAGEAR_COLOR(115,8,8);
const std::string MetaGear::TYPE="MetaGear";
const std::string MetaGear::EXTENSION=".meta";

MetaGear::MetaGear(Schema *parentSchema, std::string vname, std::string uniqueName) :
Gear(parentSchema, TYPE, uniqueName),
_metaGearName(vname),
_associatedControlPanel(0)
{
  _schema = new Schema(this);
  _schema->addSchemaEventListener(this);
}

MetaGear::~MetaGear()
{
  //TODOFOO: check this!!?
  //_schema.removeAllGears();
  //_schema->removeSchemaEventListener(this);
}

GearGui* MetaGear::createGearGui(QCanvas *canvas)
{
  //we just want to specify a specific color metagears
  return new GearGui(this, canvas, METAGEAR_COLOR);
}

bool MetaGear::ready()
{
  return true;
}

void MetaGear::internalSave(QDomDocument &doc, QDomElement &parent)
{
  std::cerr<<"metagear savEW!!"<<std::endl;
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
  //for now we will expose all inputs and outputs

  //clear plugs first, we will recreate them
  std::list<AbstractPlug*>::iterator plugIt;
  for(plugIt = _plugs.begin(); plugIt != _plugs.end(); ++plugIt)
  {
    delete(*plugIt);
  }
  _plugs.clear();

  //clear plug mapping
  _plugMapping.clear();

  std::list<Gear*> gears = _schema->getGears();

  std::list<AbstractPlug*> inputs;
  std::list<AbstractPlug*> outputs;

  for(std::list<Gear*>::iterator gearIt=gears.begin();gearIt!=gears.end();++gearIt)
  {
    (*gearIt)->getInputs(inputs, true);
    for(std::list<AbstractPlug*>::iterator plugIt=inputs.begin(); plugIt!=inputs.end(); ++plugIt)
    {
      AbstractPlug* clone = (*plugIt)->clone(this);
      clone->name((*plugIt)->fullName());
      clone->forwardPlug(*plugIt);
      addPlug(clone);
//       _plugMapping[clone]=*plugIt;
    }

    (*gearIt)->getOutputs(outputs, true);
    for(std::list<AbstractPlug*>::iterator plugIt=outputs.begin(); plugIt!=outputs.end(); ++plugIt)
    {
      AbstractPlug* clone = (*plugIt)->clone(this);
      clone->name((*plugIt)->fullName());
      clone->forwardPlug(*plugIt);
      addPlug(clone);
//       _plugMapping[clone]=*plugIt;
    }
  }

  getGearGui()->refresh();

}

void MetaGear::save(std::string filename)
{  
  QDomDocument doc("MetaGear");
    
  QDomElement metaGearElem = doc.createElement("MetaGear");
  doc.appendChild(metaGearElem);
    
  Gear::save(doc, metaGearElem);

  QFile file(filename.c_str());
  if (file.open(IO_WriteOnly))
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

bool MetaGear::load(std::string filename)
{
  QDomDocument doc("MetaGear");

  QFile file(filename.c_str());

  if (!file.open(IO_ReadOnly))
  {
    std::cout << "Fail to open file " << filename << std::endl;
    return false;
  }

  QString errMsg;
  int errLine;
  int errColumn;
  if (!doc.setContent(&file, true, &errMsg, &errLine, &errColumn))
  {
    std::cout << "parsing error in " << filename << std::endl;
    std::cout << errMsg.ascii() << std::endl;
    std::cout << "Line: " <<  errLine << std::endl;
    std::cout << "Col: " <<  errColumn << std::endl;
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

void MetaGear::onGearAdded(Schema *schema, Gear *gear)
{
  //if this event is not for our schema (it's from a child schema), do nothing
  if (schema!=_schema)
    return;
  
  if (!_associatedControlPanel)
    return;
  
  _associatedControlPanel->addControl(gear);
}

void MetaGear::onGearRemoved(Schema *schema, Gear*)
{
  //if this event is not for our schema (it's from a child schema), do nothing
  if (schema!=_schema)
    return;
}


