#include "PlugsTableItemFilename.h"
#include "AbstractPlug.h"
#include "StringType.h"

#include "SelectFilenameWidget.h"

PlugsTableItemFilename::PlugsTableItemFilename(AbstractPlug *plug, QTable * table, EditType et) :
  PlugsTableItem(plug, table, et, static_cast<StringType*>(plug->abstractDefaultType())->value().c_str())
{
  setReplaceable(false);  
}

PlugsTableItemFilename::~PlugsTableItemFilename()
{

}

QWidget *PlugsTableItemFilename::createEditor() const
{
  std::cout << "createEditor" << std::endl;
	//we need to unconst :(
	((PlugsTableItemFilename*)this)->_selectFilename = new SelectFilenameWidget(table()->viewport());
  StringType *data = static_cast<StringType*>(_plug->abstractDefaultType());

  QString filename(data->value());
	((PlugsTableItemFilename*)this)->_selectFilename->setFilename(filename);  
  
  QObject::connect(_selectFilename, SIGNAL(filenameChanged(const QString&)), table(), SLOT( doValueChanged()) );
  return _selectFilename;
}

void PlugsTableItemFilename::setContentFromEditor(QWidget *w)
{
  std::cout << "setcontentfromeditor" << std::endl;
  //if (w->inherits("QLineEdit"))
    setText(((SelectFilenameWidget*)w)->filename());
  //else
    //QTableItem::setContentFromEditor(w);
}

void PlugsTableItemFilename::setText(const QString &s)
{
  std::cout << "settext" << std::endl;
  StringType *data = static_cast<StringType*>(_plug->abstractDefaultType());
  data->setValue(s);
  
  if (_selectFilename)
    _selectFilename->setFilename(s);
  
  QTableItem::setText(s);
}



