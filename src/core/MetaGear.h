#ifndef METAGEAR_INCLUDED
#define METAGEAR_INCLUDED

#include "Gear.h"
#include "Schema.h"

class MetaGear : public Gear
{
public:  
  MetaGear(Engine* engine, std::string type, std::string name);
  virtual ~MetaGear();
  virtual Schema* getInternalSchema(){return &_schema;}

  //! is this gear a MetaGear? 
  virtual bool isMeta() const {return true;}
  bool ready();

protected:
  Schema _schema;

};

#endif
