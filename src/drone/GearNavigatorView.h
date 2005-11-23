/* GearNavigatorView.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef GearNavigatorView_INCLUDED
#define GearNavigatorView_INCLUDED

#include "qwidget.h"
#include <qlayout.h>
#include <qcombobox.h>
#include "GearListView.h"

class SchemaEditor;
class QComboBox;
class QPushButton;
class QLineEdit;

class GearNavigatorView : public QWidget
{
  Q_OBJECT
  public:
  GearNavigatorView(QWidget *parent, SchemaEditor*);
  virtual ~GearNavigatorView();
  void create();
  void focusAndClear();
	
public slots:
  void slotClearClicked();
  void textChanged(const QString&);
  void returnPressed();
private:
  	SchemaEditor* _schemaEditor;
  QVBoxLayout *_vLayout;
  QHBoxLayout *_hLayout;
  QComboBox *_comboEdit;
  QPushButton *_clearButton;
  GearListView *_gearListView;
};

#endif
