/*
 *  PlugPropertiesTableItemValue.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PLUGPROPERTIESTABLEITEMSTRING_INCLUDED
#define PLUGPROPERTIESTABLEITEMSTRING_INCLUDED

#include "PlugPropertiesTableItem.h"
#include <qlineedit.h>

class PlugPropertiesTableItemString : public PlugPropertiesTableItem
{
public:  
  PlugPropertiesTableItemString(AbstractPlug *plug, Q3Table *table, EditType et);
  virtual ~PlugPropertiesTableItemString();

  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
  void setText(const QString &s);


protected:
  QLineEdit *_lineEdit;
};
                               
#endif                               
