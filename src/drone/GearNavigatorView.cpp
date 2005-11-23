/* GearNavigatorView.cpp
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

#include "GearNavigatorView.h"
#include "SchemaEditor.h"
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qstring.h>
#include <qpushbutton.h>
#include "GearMaker.h"
#include <iostream>
#include <vector>

GearNavigatorView::GearNavigatorView(QWidget *parent,SchemaEditor* schemaEditor):QWidget(parent),_schemaEditor(schemaEditor)
{
  _vLayout = new QVBoxLayout(this, 3);
  _hLayout = new QHBoxLayout(_vLayout, 3);
  _clearButton = new QPushButton("Clear", this);
  _hLayout->addWidget(_clearButton);
  _comboEdit = new QComboBox(this);
  _comboEdit->setEditable(true);
  _comboEdit->setAutoCompletion(true); 
_comboEdit->popup();
  _hLayout->addWidget(_comboEdit,5);
  _gearListView = new GearListView(this);
  _vLayout->addWidget(_gearListView);
  QObject::connect(_clearButton, SIGNAL(clicked()), this, SLOT(slotClearClicked()));
  QObject::connect(_comboEdit,SIGNAL(textChanged(const QString&)),SLOT(textChanged(const QString&)));
  QObject::connect(_comboEdit,SIGNAL(highlighted(const QString&)),SLOT(highlighted(const QString&)));
  QObject::connect(_comboEdit->lineEdit(),SIGNAL(returnPressed()),SLOT(returnPressed()));
}

GearNavigatorView::~GearNavigatorView()
{}

void GearNavigatorView::returnPressed()
{std::cerr<<"selected:"<<_comboEdit->lineEdit()->text().latin1()<<std::endl;
_schemaEditor->addMovingGear(_comboEdit->lineEdit()->text().latin1());
_schemaEditor->setFocus();
}

void GearNavigatorView::focusAndClear()
{
  _comboEdit->lineEdit()->setFocus();

  _comboEdit->lineEdit()->clear();
}
void GearNavigatorView::textChanged(const QString& str)
{
  _comboEdit->listBox()->clear();
  QStringList sl;
  std::vector<const GearInfo*> gi;
  GearMaker::getAllGearsInfoWithNameFilter(gi,str.latin1());
  for(std::vector<const GearInfo*>::const_iterator it = gi.begin();it!=gi.end();++it)
    sl.push_back((*it)->name);
  _comboEdit->listBox()->insertStringList(sl);
}

void GearNavigatorView::create() 
{
  _gearListView->create();	
}

void GearNavigatorView::slotClearClicked()
{
  _comboEdit->lineEdit()->setText("");
}
