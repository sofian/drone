/* Gear_ListBox.h
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#include "GearControl.h"
#include "EnumType.h"

#include <string>

class Gear_ListBox : public Gear
{
public:
  static const QString SETTING_NELEMS;
  static const QString SETTING_LABELS; // XXX temporary hack to save labels

  Gear_ListBox();
  virtual ~Gear_ListBox();

  void runAudio();

  void setValue(int value);
  int getValue(){ return _VALUE_OUT->type()->value(); }
  const QString& getLabel() { return _labels[getValue()]; }

protected:

  void internalSave(QDomDocument &doc, QDomElement &gearElem);
  void internalLoad(QDomElement &gearElem);

  void onUpdateSettings();
  void onPlugConnected(AbstractPlug *plug, AbstractPlug *);
  void onPlugDisconnected(AbstractPlug* plug, AbstractPlug *);
  
private:
  PlugOut<EnumType>* _VALUE_OUT;  

  bool _acceptHint;

   // XXX temporary hack to save labels
  std::vector<QString> _labels;
};

#endif 
