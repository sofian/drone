/* SchemaEditor.cpp
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

#ifndef GEARLISTMENU_INCLUDED
#define GEARLISTMENU_INCLUDED

#include <q3popupmenu.h>
#include <string>
#include <vector>
#include <map>

#include "SchemaGui.h"

class GearGui;
class ConnectionItem;
class Engine;
class Schema;

class GearListMenu : public Q3PopupMenu
{
  Q_OBJECT
public:
  GearListMenu(QWidget *parent);
  
  void create();

signals:
  //! emitted when a gear is selected, the name of the selected gear is given
  void gearSelected(QString gearName);
  
public slots:
  //!transform the activated(int) signal into a gearSelected(QString)
  void slotMenuItemSelected(int id) { emit gearSelected(text(id));}  
  
  //!used to foward gearSelected signal to parent menu
  void slotGearSelected(QString gearName) { emit gearSelected(gearName);}
  

private:
  GearListMenu *addSubMenu(std::string name);

  std::map<std::string, GearListMenu*> _subMenus;
};

#endif
