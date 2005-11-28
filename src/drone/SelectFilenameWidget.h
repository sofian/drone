/*
 *  SelectFilenameWidget.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SelectFilenameWidget_INCLUDED
#define SelectFilenameWidget_INCLUDED

#include <qwidget.h>

class QLineEdit;
class QToolButton;
class QHBoxLayout;

class SelectFilenameWidget : public QWidget
{
	Q_OBJECT
public:  
  SelectFilenameWidget(QWidget *parent);
  virtual ~SelectFilenameWidget();

	void setFilename(const QString &filename);
	QString filename();

public slots:
	void slotBrowseClicked();		
	void lineEditTextChanged(const QString&);
	
signals:
	void filenameChanged(const QString&);

protected:
	QHBoxLayout *_hLayout;
  QLineEdit *_lineEdit;
  QToolButton *_browseButton;
};
                               
#endif                               
