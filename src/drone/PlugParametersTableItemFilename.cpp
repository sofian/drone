#include "PlugParametersTableItemFilename.h"
#include "AbstractPlug.h"
#include "StringType.h"

#include <qlineedit.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

PlugParametersTableItemFilename::PlugParametersTableItemFilename(AbstractPlug *plug, QTable * table, EditType et) :
  PlugParametersTableItem(plug, table, et, static_cast<StringType*>(plug->abstractDefaultType())->value().c_str()),
  _lineEdit(NULL)
{
  setReplaceable(false);  
}

PlugParametersTableItemFilename::~PlugParametersTableItemFilename()
{

}

QWidget *PlugParametersTableItemFilename::createEditor() const
{
  std::cout << "createEditor" << std::endl;
	//we need to unconst :(
	PlugParametersTableItemFilename *parent = ((PlugParametersTableItemFilename*)this)
	
	parent->_hLayout = new QHBoxLayout(table()->viewport(), 3);
	
  parent->_lineEdit = new QLineEdit(table()->viewport());
  _hLayout->addWidget(_lineEdit);
  parent->_browseButton = new QPushButton("...", table()->viewport());
  _hLayout->addWidget(_browseButton);
  QObject::connect(_browseButton, SIGNAL(clicked()), this, SLOT(slotBrowseClicked()));                
  StringType *data = static_cast<StringType*>(_plug->abstractDefaultType());

  _lineEdit->setText(data->value());  
  
  QObject::connect(_lineEdit, SIGNAL( textChanged(const QString&)), table(), SLOT( doValueChanged()) );
  return _lineEdit;
}

void PlugParametersTableItemFilename::setContentFromEditor(QWidget *w)
{
  std::cout << "setcontentfromeditor" << std::endl;
  if (w->inherits("QLineEdit"))
    setText(((QLineEdit*)w)->text());
  else
    QTableItem::setContentFromEditor(w);
}

void PlugParametersTableItemFilename::setText(const QString &s)
{
  std::cout << "settext" << std::endl;
  StringType *data = static_cast<StringType*>(_plug->abstractDefaultType());
  data->setValue(s);
  
  if (_lineEdit)
    _lineEdit->setText(s);
  
  QTableItem::setText(s);
}

void PlugParametersTableItemFilename::slotBrowseClicked()
{   
  if(_multiple) 
  {    
    QStringList filenames = QFileDialog::getOpenFileNames("*.*", "./", this, "Load", "Load");
    _values.clear();
    _lineEdit->setText(filenames.join(","));
  }
  else
  {
		QString filename= QFileDialog::getOpenFileName("./", "*.*", this, "Load", "Load");
		_lineEdit->setText(filename);
  }
  
}




