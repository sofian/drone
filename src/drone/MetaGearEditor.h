#ifndef METAGEAREDITOR_INCLUDED
#define METAGEAREDITOR_INCLUDED

#include <qwidget.h>
#include "GearNavigatorView.h"

class MetaGear;
class SchemaEditor;
class SchemaGui;
class Engine;
class QSplitter;
class GearListView;
class PlugPropertiesTable;

class MetaGearEditor : public QWidget
{
public:
  MetaGearEditor(QWidget *parent, MetaGear *metaGear, Engine *engine);
  ~MetaGearEditor();

  SchemaGui *schemaGui(){return _schemaGui;}
  GearNavigatorView *gearNavigatorView(){return _gearNavigatorView;}
  SchemaEditor *schemaEditor(){return _schemaEditor;}    
  
	void zoomInSchema();
	void zoomOutSchema();
	
private:
  MetaGear *_metaGear;
  SchemaGui *_schemaGui;

  QSplitter *_horizontalSplitter;
	QSplitter *_verticalSplitter;

  GearNavigatorView *_gearNavigatorView;
  PlugPropertiesTable *_plugPropertiesTable;
  SchemaEditor *_schemaEditor;

};

#endif
