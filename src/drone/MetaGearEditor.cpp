#include "MetaGearEditor.h"

#include "MetaGear.h"
#include "SchemaEditor.h"
#include "GearListView.h"
#include "PlugsTable.h"

#include <qsplitter.h>
#include <qlayout.h>
#include <qtextedit.h>

MetaGearEditor::MetaGearEditor(QWidget *parent, MetaGear *metaGear, Engine *engine) :
  QWidget(parent),
  _metaGear(metaGear)
{  
  QBoxLayout *layout = new QVBoxLayout(this, 1);

  _schemaGui = new SchemaGui(_metaGear->getInternalSchema(), engine);

  _bottomVerticalSplitter = new QSplitter(QSplitter::Vertical, this);
	
  _schemaEditor = new SchemaEditor(_bottomVerticalSplitter, _schemaGui, engine);  

  _logView = new QTextEdit(_bottomVerticalSplitter);
  _logView->setTextFormat(Qt::LogText);
  _logView->append("<font color=\"#220\">allo</font>");

  _bottomVerticalSplitter->moveToFirst(_schemaEditor);

  QObject::connect(_schemaEditor, SIGNAL(gearSelected(GearGui*)), _PlugsTable, SLOT(slotGearSelected(GearGui*)));
  
}

MetaGearEditor::~MetaGearEditor()
{
}

void MetaGearEditor::zoomInSchema()
{
	_schemaEditor->zoomIn();
}

void MetaGearEditor::zoomOutSchema()
{
	_schemaEditor->zoomOut();
}
