/*
 *  PlugsTableItemValue.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PlugsTABLEITEMVALUE_INCLUDED
#define PlugsTABLEITEMVALUE_INCLUDED

#include "PlugsTableItem.h"
#include <qlineedit.h>

class PlugsTableItemValue : public PlugsTableItem
{
public:  
  PlugsTableItemValue(AbstractPlug *plug, QTable *table, EditType et);
  virtual ~PlugsTableItemValue();

  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
  void setText(const QString &s);


protected:
  QLineEdit *_lineEdit;
};
                               
#endif                               
