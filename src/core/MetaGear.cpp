#include "MetaGear.h"
#include "GearGui.h"

#include "XMLHelper.h"

const QColor MetaGear::METAGEAR_COLOR(115,8,8);  
const std::string MetaGear::TYPE="MetaGear";

MetaGear::MetaGear(Schema *schema, std::string vname, std::string uniqueName) :
Gear(schema, TYPE, uniqueName),
_metaGearName(vname)
{
}

MetaGear::~MetaGear()
{
  //_schema.removeAllGears();
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

void MetaGear::save(QDomDocument &doc, QDomElement &parent)
{
  _schema.save(doc, parent);
}

void MetaGear::load(QDomElement &parent)
{  
  
  //find the schema node under the metagear
  QDomNode schemaNode = XMLHelper::findChildNode(parent, Schema::XML_TAGNAME);
    
  if (schemaNode.isNull())
  {
    std::cout << "Bad DroneSchema : <Schema> tag not found for metagear!" << std::endl;
    return;
  }
  
  QDomElement schemaElem = schemaNode.toElement();  
  _schema.load(schemaElem);
  
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
  
  
  std::list<Gear*> gears = _schema.getGears();
  
  std::list<AbstractPlug*> inputs;
  std::list<AbstractPlug*> outputs;

  for(std::list<Gear*>::iterator gearIt=gears.begin();gearIt!=gears.end();++gearIt)
  {
    (*gearIt)->getInputs(inputs, true);
    for(std::list<AbstractPlug*>::iterator plugIt=inputs.begin(); plugIt!=inputs.end(); ++plugIt)
    {
      AbstractPlug* clone = (*plugIt)->clone(this);
      clone->disconnectAll();
      clone->exposed(false);
      addPlug(clone);
    }

    (*gearIt)->getOutputs(outputs, true);
    for(std::list<AbstractPlug*>::iterator plugIt=outputs.begin(); plugIt!=outputs.end(); ++plugIt)
    {
      AbstractPlug* clone = (*plugIt)->clone(this);
      clone->disconnectAll();
      clone->exposed(false);
      addPlug(clone);
    }
  }
  
  getGearGui()->refresh();
  
}
