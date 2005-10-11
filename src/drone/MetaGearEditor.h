#ifndef METAGEAREDITOR_INCLUDED
#define METAGEAREDITOR_INCLUDED

#include <qwidget.h>

class MetaGear;
class PanelScrollView;
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
  
private:
  MetaGear *_metaGear;
  SchemaGui *_schemaGui;

  QSplitter *_horizontalSplitter;
	QSplitter *_verticalSplitter;

  GearListView *_gearListView;
  PlugPropertiesTable *_plugPropertiesTable;
  PanelScrollView *_panelScrollView;
  SchemaEditor *_schemaEditor;    

};

#endif
