/* GearPropertiesDialog.h
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#ifndef GEARPROPERTIESDIALOG_INCLUDED
#define GEARPROPERTIESDIALOG_INCLUDED

#include <qdialog.h>
#include <vector>

class QPushButton;
class Gear;
class QVBoxLayout;
class PropertyControl;
class Property;

class GearPropertiesDialog : public QDialog
{
  Q_OBJECT
  public:

  GearPropertiesDialog(QWidget *parent, Gear *gear);
  ~GearPropertiesDialog();

public slots:
  void slotOK();

private:

  void addControl(Property *property);

  Gear* _gear;
  QVBoxLayout* _verticalLayout;
  std::vector<PropertyControl*> _propertyControls;

  QPushButton *_okButton;
  QPushButton *_cancelButton;

};

#endif

