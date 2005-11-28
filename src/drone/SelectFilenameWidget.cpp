#include "SelectFilenameWidget.h"

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtoolbutton.h>

SelectFilenameWidget::SelectFilenameWidget(QWidget *parent) :
	QWidget(parent)
{
	_hLayout = new QHBoxLayout(this, 1, 0);
	
	_lineEdit = new QLineEdit(this);
	_hLayout->addWidget(_lineEdit);
	_browseButton = new QToolButton(this);
	_hLayout->addWidget(_browseButton);
	QObject::connect(_browseButton, SIGNAL(clicked()), this, SLOT(slotBrowseClicked()));
  QObject::connect(_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT( lineEditTextChanged(const QString&)) );	
}

SelectFilenameWidget::~SelectFilenameWidget()
{
}

void SelectFilenameWidget::setFilename(const QString &filename)
{
	_lineEdit->setText(filename);
}

QString SelectFilenameWidget::filename()
{
	return _lineEdit->text();
}

void SelectFilenameWidget::slotBrowseClicked()
{   
	QStringList filenames = QFileDialog::getOpenFileNames("*.*", "./", this, "Load", "Load");
	_lineEdit->setText(filenames.join(","));  
}

void SelectFilenameWidget::lineEditTextChanged(const QString& text)
{
	emit filenameChanged(text);
}



