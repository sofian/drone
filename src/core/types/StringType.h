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


class StringType : public AbstractType
{
public:
  StringType(QString defaultValue="", bool isAFilename=false)
	: AbstractType(),
	_value(defaultValue),
	_isAFilename(isAFilename)
{}
  
  virtual ~StringType() {}

  TYPE_BASE_METHODS(StringType, (123, 173, 241))

	void save(QDomDocument &doc, QDomElement &parent) const;
	void load(QDomElement &typeElem, Drone::LoadingModeFlags lmf);
	  
	bool isAFilename(){return _isAFilename;}
  void setValue(const QString& value) { _value = value; }
	const QString& value() const { return _value; }
  
private:
	QString _value;
	bool _isAFilename;
};

#endif






