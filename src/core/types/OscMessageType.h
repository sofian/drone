/* OscType.h
* Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#ifndef __OSCMESSAGETYPE_INCLUDED
#define __OSCMESSAGETYPE_INCLUDED

#include "AbstractType.h"
#include "StringType.h"
#include "ListType.h"

class OscMessageType : public AbstractType
{
public:
  OscMessageType() : 
    AbstractType()
  {
    addSubType(_path);
    addSubType(_argList);
  }
  
  virtual ~OscMessageType() 
  {
  }
  

  TYPE_BASE_METHODS(OscMessageType, (149, 153, 162))

	void setPath(const StringType &path) {_path = path;}
	void setArgs(const ListType &args) {_argList = args;}
	
	StringType path() const {return _path;}
	ListType args() const {return _argList;}
	
private:
  StringType _path;
  ListType _argList;
};

#endif






