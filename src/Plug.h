/* Plug.h
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

#ifndef __PLUG_INCLUDED
#define __PLUG_INCLUDED

#include "AbstractPlug.h"

/**
 * This class is a template instance of an <code>AbstractPlug</code>. 
 */

template <class T>
class PlugOut : public AbstractPlug
{
public:
  PlugOut(Gear* parent, std::string name, T* type = new T())
  : AbstractPlug(parent, OUT, name, type)
  {
    _type = _internalType = type;
  }

  virtual ~PlugOut()
  {
  }

  T* type() { return _type;}
  T* defaultType() { return _internalType; }
  T* hintType() { return _internalType; }
  
  const T* type() const { return _type;}
  const T* defaultType() const { return _internalType; }
  const T* hintType() const { return _internalType; }

//  void setPlugState(ePlugState ps){_plugState=ps;}
//  ePlugState plugState(){return _plugState;}

  void init() {}

private:
  T *_type, *_internalType;
  //! state of the plug : when gears don't update a plug, _plugState is == SLEEPING to tell the plug is inactive.
  //! (e.g for a switch, all plugs other than active one are == SLEEPING) If the plug is an out, the value is 
  //! updated by the parent gear. If its an in, the value is copied from the connected out plug.
  ePlugState _plugState;

};


template <class T>
class PlugIn : public AbstractPlug
{
public:
  PlugIn(Gear* parent, std::string name, T* type = new T())
  : AbstractPlug(parent, IN, name, type)
  {
     _type = _internalType = type;
  }
 
  virtual ~PlugIn()
  {}

  virtual void onConnection(AbstractPlug *plug)
  {
    setType(static_cast<const T*>(plug->abstractType()));
  }

  virtual void onDisconnection(AbstractPlug *)
  {
    setType(_internalType);
  }

  void init() {}

  T* defaultType() { return _internalType; }
  T* hintType() { return _internalType; }

  const T* type() const { return _type;}
  const T* defaultType() const { return _internalType; }
  const T* hintType() const { return _internalType; }

//   ePlugState plugState()
//     {
//       if(connected())
//       return {
//       }
//     }

protected:
  void setType(const T *type)
  {
    _abstractType = _type = type;
  }

private:
  const T *_type;
  T *_internalType;
};


#endif  //  __PLUG_INCLUDED
