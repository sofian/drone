/* MetaGearMaker.h
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

#ifndef METAGEAR_MAKER_INCLUDED
#define METAGEAR_MAKER_INCLUDED

#include <string>
#include <vector>
#include <qdir.h>
#include <qfileinfo.h>

#include "MetaGear.h"

class MetaGearMaker  
{
public:
  MetaGearMaker();
  ~MetaGearMaker();

  static const std::string METAGEAR_PATH;

  static MetaGear* makeMetaGear(Schema *schema, std::string type, std::string uniqueName);
  static const std::vector<QFileInfo*> &getAllMetaGearsPath();
  static void parseMetaGears();  
  static void parseSubDirs(QDir dir);
private:
  static std::vector<QFileInfo*> *_registry;//!qfileinfo  containing relative path to metagears
  static MetaGearMaker _registerMyself;  
};

#endif
