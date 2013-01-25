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
  PlugOut(Gear* parent, QString name, bool mandatory, T* type = new T())
  : AbstractPlug(parent, OUT, name, type, mandatory)
  {    
    _forwardPlug = 0;
  }

  virtual ~PlugOut()
  {
  }

  T* type() { return static_cast<T*>(_abstractType);}

  T* defaultType() { return static_cast<T*>(_abstractDefaultType);}
  T* hintType() { return static_cast<T*>(_abstractDefaultType);}

  const T* type() const { return static_cast<const T*>(_abstractType);}
  const T* defaultType() const { return static_cast<const T*>(_abstractDefaultType);}
  const T* hintType() const { return static_cast<const T*>(_abstractDefaultType);}


  virtual bool ready() const
  {
    if (_mandatory)
      if (!connected())
        return false;

    return true;
  }

  void init() {}

  AbstractPlug *clone(Gear* parent)
  {
    return new PlugOut<T>(parent, name(), _mandatory);
  }

};


template <class T>
class PlugIn : public AbstractPlug
{
public:
  PlugIn(Gear* parent, QString name, bool mandatory, T* type = new T())
  : AbstractPlug(parent, IN, name, type, mandatory)
  {
    _forwardPlug = 0;
  }

  virtual ~PlugIn()
  {}


  virtual bool ready() const
  {   
    if (_mandatory)
      if (!connected())
        return false;

    if (connected())
    {
      if (_mandatory && !firstConnectedPlug()->parent()->ready())
        return false;

      return !(firstConnectedPlug()->sleeping()); 
    }

    //not mandatory and not connected
    return true;
  }

  virtual void onConnection(AbstractPlug *plug)
  {
    // for other plug
    AbstractPlug * deepestOtherPlug = 0;
    for (deepestOtherPlug = plug; deepestOtherPlug->forwardPlug() != 0; deepestOtherPlug = deepestOtherPlug->forwardPlug()) ;

    //for this plug
    AbstractPlug * deepestPlug = 0;
    for (deepestPlug = this; deepestPlug->forwardPlug() != 0; deepestPlug = deepestPlug->forwardPlug()) ;

    dynamic_cast<PlugIn<T>*>(deepestPlug)->setType(deepestOtherPlug->abstractType());
  }

  virtual void onDisconnection(AbstractPlug *)
  {
    AbstractPlug * deepestPlug = 0;
    for (deepestPlug = this; deepestPlug->forwardPlug() != 0; deepestPlug = deepestPlug->forwardPlug()) ;

    dynamic_cast<PlugIn<T>*>(deepestPlug)->setType(_abstractDefaultType);

/*    if(_forwardPlug)
      dynamic_cast<PlugIn<T>*>(_forwardPlug)->setType(_internalType);
    else
      setType(_internalType);*/
  }

  void init() {}

  T* defaultType() { return static_cast<T*>(_abstractDefaultType);}
  T* hintType() { return static_cast<T*>(_abstractDefaultType);}

  T* type() const { return static_cast<T*>(_abstractType);}
  const T* defaultType() const { return static_cast<const T*>(_abstractDefaultType);}
  const T* hintType() const { return static_cast<const T*>(_abstractDefaultType);}

  T* cloneType() const { return static_cast<T*>(_abstractType->clone()); }

  AbstractPlug *clone(Gear* parent)
  {
    PlugIn<T>* clonePlug = new PlugIn<T>(parent, name(), _mandatory);
    return clonePlug;
  }


protected:
  void setType(AbstractType *type)
  {
    if (type == NULL)
      qCritical() << Q_FUNC_INFO << " called with type = NULL";
    _abstractType = type;
  }

};

#endif  //  __PLUG_INCLUDED
