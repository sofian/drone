/* EnumType.h
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

#ifndef ENUMTYPE_INCLUDED
#define ENUMTYPE_INCLUDED

#include "AbstractType.h"
#include <vector>

class EnumType : public AbstractType
{
public:
  EnumType(int size = 0, int defaultValue=0)
  {
    _names.resize(size);
    setValue(defaultValue);
  }
  
  virtual ~EnumType() {}
  
  virtual std::string name() const { return "EnumType"; }
  virtual QColor color() const { return QColor(238,19,19); }
  
  void setValue(int value) {
    ASSERT_ERROR(_names.empty() || (0 <= value && value < _names.size()));
    _value = value;
  }
  int value() const { return _value; }
  
  operator int () const {return _value;}
  
  void setName(int value, const std::string& name) { _names[value] = name; }
  const std::string& name(int value) const { return _names[value]; }
  
private:
  int _value;
  std::vector<std::string> _names;
};

#endif
