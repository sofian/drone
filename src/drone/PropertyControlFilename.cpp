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
#include <qfiledialog.h>

PropertyControlFilename::PropertyControlFilename(QWidget *parent, Property *property) :
PropertyControl(parent, property)
{
  _hLayout = new QHBoxLayout(this, 3);

  _hLayout->addWidget(new QLabel(property->name().c_str(), this, ""));
  _lineEdit = new QLineEdit(this);
  _hLayout->addWidget(_lineEdit);
  _browseButton = new QPushButton("...", this);
  _hLayout->addWidget(_browseButton);
  QObject::connect(_browseButton, SIGNAL(clicked()), this, SLOT(slotBrowseClicked()));                

  _lineEdit->setText(property->valueStr().c_str());
}

PropertyControlFilename::~PropertyControlFilename()
{
}

void PropertyControlFilename::save()
{
  _property->valueStr(_lineEdit->text().ascii());    
}

void PropertyControlFilename::slotBrowseClicked()
{    
  QString filename = QFileDialog::getOpenFileName("./", "*.*", this, "Load", "Load");
  _lineEdit->setText(filename);
}




