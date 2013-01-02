/* StringType.h
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

#ifndef __StringType_INCLUDED
#define __StringType_INCLUDED

#include "AbstractType.h"
#include <string>

class StringType : public AbstractType
{
public:
  StringType(std::string defaultValue="")
	: AbstractType(),
	_value(defaultValue)
  {}
  
  virtual ~StringType() {}
  
  TYPE_BASE_METHODS(StringType, (123, 173, 241))

  void setValue(std::string value) { _value = value; }
  std::string value() const { return _value; }

private:
	std::string _value;
};

#endif






