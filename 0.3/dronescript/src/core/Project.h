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

#include <string>

class QDomDocument;
class QDomElement;
class SchemaGui;

class Project  
{

public:

  Project(SchemaGui* mainSchemaGui);

  bool save();
  bool saveAs(std::string filename);
  bool load(std::string filename);
  std::string projectName(){return _projectName;}
        
  void newProject();

protected:

  SchemaGui* _mainSchemaGui;
  std::string _projectName;
  
};

#endif
