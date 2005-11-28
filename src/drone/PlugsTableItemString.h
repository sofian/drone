/*
 *  PlugsTableItemValue.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PlugsTABLEITEMSTRING_INCLUDED
#define PlugsTABLEITEMSTRING_INCLUDED

#include "PlugsTableItem.h"
#include <qlineedit.h>

class PlugsTableItemString : public PlugsTableItem
{
public:  
  PlugsTableItemString(AbstractPlug *plug, QTable *table, EditType et);
  virtual ~PlugsTableItemString();

  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
  void setText(const QString &s);


protected:
  QLineEdit *_lineEdit;
};
                               
#endif                               
