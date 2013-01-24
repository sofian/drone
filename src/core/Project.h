/* Project.h
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

#ifndef PROJECT_INCLUDED
#define PROJECT_INCLUDED

#include <QString>
#include "DroneGlobals.h"

class QDomDocument;
class QDomElement;
class Schema;

class Project  
{
public:
  
  Project(Schema* mainSchema);


  bool save();
  bool saveAs(QString filename);
  bool load(const QDomDocument& filename, Drone::LoadingModeFlags lmf);
  bool loadFromFile(QString filename);
  QDomDocument getSnapshot();
  QString projectName(){return _projectName;}
        
  void newProject();

protected:

  Schema* _mainSchema;
  QString _projectName;
  
};

#endif
