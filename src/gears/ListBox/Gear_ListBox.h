/* Gear_ListBox.h
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

#ifndef GEAR_LISTBOX_INCLUDED
#define GEAR_LISTBOX_INCLUDED

#include "Gear.h"
#include "EnumType.h"

#include <string>

class Gear_ListBox : public Gear  
{
public:
  static const std::string SETTING_NELEMS;
  static const std::string SETTING_LABELS; // XXX temporary hack to save labels

  Gear_ListBox(Schema *schema, std::string uniqueName);
  virtual ~Gear_ListBox();

  void save(QDomDocument &doc, QDomElement &gearElem);
  void load(QDomElement &gearElem);

  void runAudio();

  void setValue(int value);
  int getValue(){ return _VALUE_OUT->type()->value(); }
  const std::string& getLabel() { return _labels[getValue()]; }

  bool ready();

protected:
  GearGui* createGearGui(QCanvas *canvas);//we want a slider so we overload
  void onUpdateSettings();
  void onPlugConnected(AbstractPlug *plug, AbstractPlug *);
  void onPlugDisconnected(AbstractPlug* plug, AbstractPlug *);
  
private:
  PlugOut<EnumType>* _VALUE_OUT;  

  bool _acceptHint;

   // XXX temporary hack to save labels
  std::vector<std::string> _labels;
};

#endif 
