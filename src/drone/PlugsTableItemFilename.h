/*
 *  PlugsTableItemFilename.h
 *  drone
 *
 *  Created by foogy on 11/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PlugsTableItemFilename_INCLUDED
#define PlugsTableItemFilename_INCLUDED

#include "PlugsTableItem.h"

class SelectFilenameWidget;

class PlugsTableItemFilename : public PlugsTableItem
{
public:  
  PlugsTableItemFilename(AbstractPlug *plug, QTable *table, EditType et);
  virtual ~PlugsTableItemFilename();

  QWidget *createEditor() const;
  void setContentFromEditor(QWidget *w);
  void setText(const QString &s);
protected:
	SelectFilenameWidget *_selectFilename;
};
                               
#endif                               
