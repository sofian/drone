#include "PropertyControlFilename.h"
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

PropertyControlFilename::PropertyControlFilename(QWidget *parent, Property *property) :
PropertyControl(parent, property)
{
  _hLayout = new QHBoxLayout(this, 3);

  _hLayout->addWidget(new QLabel(property->name().c_str(), this, ""));
  _lineEdit = new QLineEdit(this);
  _hLayout->addWidget(_lineEdit);
  _browseButton = new QPushButton("...", this);
  _hLayout->addWidget(_browseButton);
  QObject::connect(_browseButton, SIGNAL(clicked()), this, SLOT(slotBrowseClicked()));                

  _lineEdit->setText(property->valueStr().c_str());
}

PropertyControlFilename::~PropertyControlFilename()
{
}

void PropertyControlFilename::save()
{
  _property->valueStr(_lineEdit->text().ascii());    
}

void PropertyControlFilename::slotBrowseClicked()
{    
  QString filename = QFileDialog::getOpenFileName("./", "*.*", this, "Load", "Load");
  _lineEdit->setText(filename);
}




