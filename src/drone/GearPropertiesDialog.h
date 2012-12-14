/* GearPropertiesDialog.h
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

#ifndef GEARPROPERTIESDIALOG_INCLUDED
#define GEARPROPERTIESDIALOG_INCLUDED

#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <vector>

class QPushButton;
class Gear;
class Q3VBoxLayout;
class PropertyControl;
class Property;
class Engine;

class GearPropertiesDialog : public QDialog
{
  Q_OBJECT
  public:
	
  GearPropertiesDialog(QWidget *parent, Gear *gear, Engine *engine);
  ~GearPropertiesDialog();

public slots:
  void slotOK();

private:

  void addControl(Property *property);

  Gear* _gear;
  Engine* _engine;

  Q3VBoxLayout* _verticalLayout;
  std::vector<PropertyControl*> _propertyControls;
  QPushButton *_okButton;

  QPushButton *_cancelButton;
};

#endif

