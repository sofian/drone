/* Gear_TA_ContextSelector.h
 * Copyright (C) 2005 Jean-Sebastien Senecal
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

#ifndef GEAR_TA_CONTEXTSELECTOR_INCLUDED
#define GEAR_TA_CONTEXTSELECTOR_INCLUDED


#include "Gear.h"
#include "ValueType.h"

class Gear_TA_ContextSelector : public Gear
{
  static const int MAX_CONTEXTS;
  
public:
  Gear_TA_ContextSelector(Schema *schema, std::string uniqueName);
  virtual ~Gear_TA_ContextSelector();

  void runVideo();

public:
  PlugIn<ValueType> *_INNOCENCE_IN;
  PlugIn<ValueType> *_CHANNEL_IN;
  PlugIn<ValueType> *_BANG_IN;

  PlugOut<ValueType>* _INNOCENCE_0_OUT;
  PlugOut<ValueType>* _CHANNEL_0_OUT;
  PlugOut<ValueType>* _BANG_0_OUT;
  PlugOut<ValueType>* _INNOCENCE_1_OUT;
  PlugOut<ValueType>* _CHANNEL_1_OUT;
  PlugOut<ValueType>* _BANG_1_OUT;
  
protected:
  int _currentContext;
};

#endif
