/*
 *  PlugPropertiesTableItemValue.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PLUGPROPERTIESTABLEITEMVALUE_INCLUDED
#define PLUGPROPERTIESTABLEITEMVALUE_INCLUDED

#include "PlugPropertiesTableItem.h"
#include <qlineedit.h>

class PlugPropertiesTableItemValue : public PlugPropertiesTableItem
{
public:  
  PlugPropertiesTableItemValue(AbstractPlug *plug, Q3Table *table, EditType et);
  virtual ~PlugPropertiesTableItemValue();

  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
  void setText(const QString &s);


protected:
  QLineEdit *_lineEdit;
};
                               
#endif                               
