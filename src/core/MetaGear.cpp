#include "MetaGear.h"
#include "GearGui.h"

const QColor MetaGear::METAGEAR_COLOR(115,8,8);  
const std::string MetaGear::TYPE="MetaGear";

MetaGear::MetaGear(Schema *schema, std::string vname, std::string uniqueName) :
Gear(schema, TYPE, uniqueName)
{
  //set the name of the gear to the name of the metagear
  name(vname);
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
  _schema.load(parent);
}
