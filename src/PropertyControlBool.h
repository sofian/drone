/* PropertyControlBool.h
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#ifndef PROPERTYCONTROLBOOL_INCLUDED
#define PROPERTYCONTROLBOOL_INCLUDED

#include "PropertyControl.h"

class QCheckBox;


class PropertyControlBool : public PropertyControl
{
public:
  PropertyControlBool(QWidget *parent, Property *property);
  virtual ~PropertyControlBool();

  void save();


private:

  QHBoxLayout *_hLayout;
  QCheckBox *_checkBox;

};

#endif
