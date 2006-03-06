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

#ifndef METAGEARLISTMENU_INCLUDED
#define METAGEARLISTMENU_INCLUDED

#include <qpopupmenu.h>
#include <qfileinfo.h>
#include <string>
#include <vector>
#include <map>

#include "SchemaGui.h"

class GearGui;
class ConnectionItem;
class Engine;
class Schema;

class MetaGearListMenu : public QPopupMenu
{
  Q_OBJECT
public:
  MetaGearListMenu(QWidget *parent, MetaGearListMenu* parentMetaGearListMenu=NULL);
  virtual ~MetaGearListMenu();
  
  void create();

signals:
  //! emitted when a metagear is selected, the name of the selected gear is given
  void metaGearSelected(QFileInfo *metaGearFileInfo);
  
public slots:
  //!transform the activated(int) signal into a metagearSelected(QFileInfo*)
  void slotMenuItemSelected(int id); 
  
  //!used to foward gearSelected signal to parent menu
  void slotMetaGearSelected(QFileInfo *metaGearFileInfo) { emit metaGearSelected(metaGearFileInfo);}
  

private:
  MetaGearListMenu *addSubMenu(std::string name);
  MetaGearListMenu *_parentMetaGearListMenu;

  std::map<std::string, MetaGearListMenu*> _subMenus;
  std::map<int, QFileInfo*> *_menuItemIndexfileInfoMap;//!mapping between menu item and corresponding metagears qfileinfo, this map is shared between submenus
};

#endif
