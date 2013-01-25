/* ValueType.h
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

#ifndef __VALUETYPE_INCLUDED
#define __VALUETYPE_INCLUDED

#include "AbstractType.h"

class ValueType : public AbstractType
{
public:
  ValueType(float defaultValue=0.0f,
            float minValue=-1.0f,
            float maxValue=1.0f)
    : AbstractType(),
	    _value(defaultValue),
      _minValue(minValue),
      _maxValue(maxValue)
    {}
  
  virtual ~ValueType() {}
    
  TYPE_BASE_METHODS(ValueType, (157, 208, 50))

	void save(QDomDocument &doc, QDomElement &parent) const;
	void load(QDomElement &typeElem);
	  
  void setValue(float value) { _value = value; }
  float value() const { return _value; }
  int intValue() const { return (int)_value; }
  bool boolValue() const { return (bool)_value; }
  
  //operator float () const {return _value;}
  
  void setMinValue(float minValue) { _minValue = minValue; }
  void setMaxValue(float maxValue) { _maxValue = maxValue; }

  float minValue() const { return _minValue; }
  float maxValue() const { return _maxValue; }

private:
  float _value, _minValue, _maxValue;
};

#endif






