#include "MetaGearEditor.h"

#include "MetaGear.h"
#include "SchemaEditor.h"
#include "PanelScrollView.h"
#include "GearTreeView.h"
#include "PlugPropertiesTable.h"

#include <qsplitter.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3BoxLayout>

MetaGearEditor::MetaGearEditor(QWidget *parent, MetaGear *metaGear, Engine *engine) :
  QWidget(parent),
  _metaGear(metaGear)
{  
  Q3BoxLayout *layout = new Q3VBoxLayout(this, 1);

  _schemaGui = new SchemaGui(_metaGear->getInternalSchema(), engine);

/*
  _horizontalSplitter = new QSplitter(QSplitter::Horizontal, this);

  _panelScrollView = new PanelScrollView(_horizontalSplitter);
  _schemaEditor = new SchemaEditor(_horizontalSplitter, _schemaGui, engine, _panelScrollView);  

  _horizontalSplitter->moveToFirst(_schemaEditor);
  
  layout->addWidget(_horizontalSplitter);

  //add the edited metagear to the panelScrollView
  _panelScrollView->addControlPanel(metaGear);
*/

  _horizontalSplitter = new QSplitter(Qt::Horizontal, this);
  _verticalSplitter = new QSplitter(Qt::Vertical, _horizontalSplitter);

  
  //_panelScrollView = new PanelScrollView(_horizontalSplitter);
  _panelScrollView = NULL;
  _gearTreeView = new GearTreeView(_verticalSplitter);
  _gearTreeView->create();
  _plugPropertiesTable = new PlugPropertiesTable(_verticalSplitter);
  _schemaEditor = new SchemaEditor(_horizontalSplitter, _schemaGui, engine, _panelScrollView);  
  
  _horizontalSplitter->moveToFirst(_verticalSplitter);
  
  layout->addWidget(_horizontalSplitter);
  
  //add the edited metagear to the panelScrollView
  //_panelScrollView->addControlPanel(metaGear);
	
  QObject::connect(_schemaEditor->scene(), SIGNAL(gearSelected(GearGui*)), _plugPropertiesTable, SLOT(slotGearSelected(GearGui*)));
  
}

MetaGearEditor::~MetaGearEditor()
{
}

