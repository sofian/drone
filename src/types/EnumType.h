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
  EnumType(int size = 1, int defaultValue=0)
  {
    _labels.resize(size);
    setValue(defaultValue);
  }
  
  virtual ~EnumType() {}
  
  virtual std::string name() const { return "EnumType"; }
  virtual QColor color() const { return QColor(238,19,19); }
  
  void setValue(int value) {
    ASSERT_ERROR(_labels.empty() || (0 <= value && value < (int)_labels.size()));
    _value = value;
  }
  int value() const { return _value; }
  const std::string& label() const { return _labels[_value]; }
  size_t size() const { return _labels.size(); }
  void resize(size_t size) { _labels.resize(size); }

  operator int () const {return _value;}
  
  void setLabel(int value, const std::string& label) { _labels[value] = label; }
  const std::string& label(int value) const { return _labels[value]; }
  
private:
  int _value;
  std::vector<std::string> _labels;
};

#endif
