#include "SchemaGui.h"

#include "Schema.h"
#include "Gear.h"
#include "GearGui.h"
#include "ConnectionItem.h"

const int SchemaGui::DEFAULT_CANVAS_SIZE_X = 2048;
const int SchemaGui::DEFAULT_CANVAS_SIZE_Y = 2048;


SchemaGui::SchemaGui(Schema *schema, Engine *engine) :
  QCanvas(DEFAULT_CANVAS_SIZE_X, DEFAULT_CANVAS_SIZE_Y),
  _schema(schema),
  _engine(engine)
{
  
  //todo various background for metagear and main schema
  setBackgroundColor(QColor(107,124,153));

  //add gearguis
  std::list<Gear*> gears = _schema->getGears();

  GearGui *gearGui=NULL;
  for (std::list<Gear*>::iterator it=gears.begin();it!=gears.end();++it)
  {
    gearGui=(*it)->getGearGui();
    gearGui->setCanvas(this);
    gearGui->show();
  }


  //add connectionItems
  std::list<Schema::Connection*> connections;
  _schema->getAllConnections(connections);
  ConnectionItem *connectionItem=NULL;
  PlugBox *sourcePlugBox;
  PlugBox *destPlugBox;
  Gear *gearA;
  Gear *gearB;

  for (std::list<Schema::Connection*>::iterator it = connections.begin(); it != connections.end(); ++it)
  {
    connectionItem = new ConnectionItem(this);

    gearA = _schema->getGearByName((*it)->gearA());
    sourcePlugBox = gearA->getGearGui()->getOutputPlugBox((*it)->output());

    gearB = _schema->getGearByName((*it)->gearB());
    destPlugBox = gearB->getGearGui()->getInputPlugBox((*it)->input());

    connectionItem->createConnectionLineOnly(sourcePlugBox, destPlugBox);

    connectionItem->show();

    delete (*it);//free

  }

  update();
}

SchemaGui::~SchemaGui()
{
}

void SchemaGui::addGear(std::string type, int x, int y)
{    
  addGear(type, _schema->getNewGearName(type), x, y);
}

void SchemaGui::addGear(std::string type, std::string name, int x, int y)
{            
  Gear *gear = _schema->addGear(_engine, type, name);    
  GearGui *gearGui = gear->getGearGui();    

  gearGui->setCanvas(this);    
  gearGui->move(x,y);    
  gearGui->setZ(0);
  gearGui->show();
  update();
}

void SchemaGui::removeGear(GearGui* gearGui)
{
  Gear* gear = gearGui->gear();
  delete gearGui;

  _engine->scheduleGearDeletion(gear);

  update();
}

void SchemaGui::clearSchema()
{

  QCanvasItemList l=allItems();

  for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
    (*it)->hide();


  //_schema->clear();
  update();

}

void SchemaGui::loadSchema(std::string filename)
{
}

void SchemaGui::saveSchema(std::string filename)
{
}

