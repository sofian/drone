#include "MetaGear.h"


MetaGear::MetaGear(Engine* engine, std::string type, std::string name):Gear(engine, type, name)
{
}

MetaGear::~MetaGear()
{
  //_schema.removeAllGears();
}

bool MetaGear::ready()
{
  return true;
}
