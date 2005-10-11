/* AbstractType.h
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

#ifndef __ABSTRACTTYPE_INCLUDED
#define __ABSTRACTTYPE_INCLUDED

#include <qcolor.h>
#include <iostream>
#include <string>
#include <vector>

class AbstractType
{
public:
  AbstractType(){}
  
		
	
  virtual ~AbstractType() {}

  virtual QColor color() const = 0;

  int nSubTypes() const { return _subTypes.size();}
  const AbstractType* getSubType(int i) const { return _subTypes[i]; }
  const AbstractType* addSubType(const AbstractType &subType)
  {
    _subTypes.push_back(&subType);
    return &subType;
  }
	
  virtual std::string typeName() const=0;
  bool typeOf(AbstractType &other) const
  {
      return other.typeName() == typeName();
  }
  
protected:
  std::vector<const AbstractType*> _subTypes;
	
private:
};


#endif //__ABSTRACTTYPE_INCLUDED

