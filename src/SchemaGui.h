#ifndef SCHEMAEGUI_INCLUDED
#define SCHEMAEGUI_INCLUDED

#include <qcanvas.h>

#include "Schema.h"

class GearGui;
class Engine;
               
class SchemaGui : public QCanvas
{
public:
  SchemaGui(Schema *schema, Engine *engine);
  ~SchemaGui();

  void addGear(std::string type, std::string name, int x, int y);
  void addGear(std::string type, int x, int y);
  void removeGear(GearGui* gearGui);
  
  void clearSchema();
  void loadSchema(std::string filename);
  void saveSchema(std::string filename);


private:
  static const int DEFAULT_CANVAS_SIZE_X;
  static const int DEFAULT_CANVAS_SIZE_Y;
  
  Schema *_schema;
  Engine *_engine;
};

#endif
