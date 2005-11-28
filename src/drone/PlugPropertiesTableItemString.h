/*
 *  PlugParametersTableItemValue.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PLUGParametersTABLEITEMSTRING_INCLUDED
#define PLUGParametersTABLEITEMSTRING_INCLUDED

#include "PlugParametersTableItem.h"
#include <qlineedit.h>

class PlugParametersTableItemString : public PlugParametersTableItem
{
public:  
  PlugParametersTableItemString(AbstractPlug *plug, QTable *table, EditType et);
  virtual ~PlugParametersTableItemString();

  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
  void setText(const QString &s);


protected:
  QLineEdit *_lineEdit;
};
                               
#endif                               
