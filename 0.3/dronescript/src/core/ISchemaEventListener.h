#ifndef ISCHEMAEVENTLISTENER_INCLUDED
#define ISCHEMAEVENTLISTENER_INCLUDED

class Gear;
class Schema;

class ISchemaEventListener
{
public:
  virtual void onGearAdded(Schema *schema, Gear *gear)=0;
  virtual void onGearRemoved(Schema *schema, Gear *gear)=0;
};

#endif

