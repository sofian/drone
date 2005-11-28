/*
 *  PlugParametersTableItemFilename.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PlugParametersTableItemFilename_INCLUDED
#define PlugParametersTableItemFilename_INCLUDED

#include "PlugParametersTableItem.h"
class QLineEdit;
class QPushButton;
class QHBoxLayout;

class PlugParametersTableItemFilename : public PlugParametersTableItem
{
public:  
  PlugParametersTableItemFilename(AbstractPlug *plug, QTable *table, EditType et);
  virtual ~PlugParametersTableItemFilename();

  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
  void setText(const QString &s);

protected:
	QHBoxLayout *_hLayout;
  QLineEdit *_lineEdit;
  QPushButton *_browseButton;
};
                               
#endif                               
