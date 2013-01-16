/* PropertyControlFilename.cpp
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

#include "PropertyControlFilename.h"
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <q3filedialog.h>
//Added by qt3to4:
#include <Q3HBoxLayout>

#include <iostream>

PropertyControlFilename::PropertyControlFilename(QWidget *parent, Property *property, bool multiple) :
PropertyControl(parent, property), _multiple(multiple)
{
  _hLayout = new Q3HBoxLayout(this, 3);

  _hLayout->addWidget(new QLabel(property->name(), this, ""));
  _lineEdit = new QLineEdit(this);
  _hLayout->addWidget(_lineEdit);
  _browseButton = new QPushButton("...", this);
  _hLayout->addWidget(_browseButton);
  QObject::connect(_browseButton, SIGNAL(clicked()), this, SLOT(slotBrowseClicked()));                

  _lineEdit->setText(property->valueStr());  
}

PropertyControlFilename::~PropertyControlFilename()
{
}

void PropertyControlFilename::save()
{
  if (_multiple)
    _property->valueStrList(_values);
  else
    _property->valueStr(_lineEdit->text());        
}

void PropertyControlFilename::slotBrowseClicked()
{   
  if(_multiple) 
  {    
    QStringList filenames = Q3FileDialog::getOpenFileNames("*.*", "./", this, "Load", "Load");
    _values.clear();
    foreach ( QString str,filenames) 
        _values<<str;
    _lineEdit->setText(filenames.join(","));
  }
  else
  {
     QString filename= Q3FileDialog::getOpenFileName("./", "*.*", this, "Load", "Load");
     _lineEdit->setText(filename);
  }
  
}




