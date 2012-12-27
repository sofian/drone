/* TA_DataType.h
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

#ifndef __TA_DataType_INCLUDED
#define __TA_DataType_INCLUDED

#include "AbstractType.h"
#include "TA_CityGraph.h"
#include <string>

class TA_DataType : public AbstractType, public TA_CityGraph
{
public:  
  TA_DataType()
    : AbstractType(),
      TA_CityGraph()
  {
  }
  
  virtual ~TA_DataType() {}
  
  virtual std::string typeName() const { return "TA_Data"; }
  virtual QColor color() const { return QColor(204, 87, 71); }
  
  void loadData(const std::string& filename) { }
  
};

#endif






