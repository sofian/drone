#include "MetaGearEditor.h"

#include "MetaGear.h"
#include "SchemaEditor.h"
#include "PanelScrollView.h"

#include <qsplitter.h>
#include <qlayout.h>

MetaGearEditor::MetaGearEditor(QWidget *parent, MetaGear *metaGear, Engine *engine) :
  QWidget(parent),
  _metaGear(metaGear)
{  
  QBoxLayout *layout = new QVBoxLayout(this, 1);

  SchemaGui *_schemaGui = new SchemaGui(_metaGear->getInternalSchema(), engine);

  _horizontalSplitter = new QSplitter(QSplitter::Horizontal, this);

  _panelScrollView = new PanelScrollView(_horizontalSplitter);
  _schemaEditor = new SchemaEditor(_horizontalSplitter, _schemaGui, engine, _panelScrollView);  

  _horizontalSplitter->moveToFirst(_schemaEditor);
  
  layout->addWidget(_horizontalSplitter);

  //add the edited metagear to the panelScrollView
  _panelScrollView->addControlPanel(metaGear);
}

MetaGearEditor::~MetaGearEditor()
{
}
