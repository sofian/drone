#ifndef __PLUG_INCLUDED
#define __PLUG_INCLUDED

#include "AbstractPlug.h"

template <class T>
class PlugOut;
template <class T>
class PlugIn;

template <class T>
class Plug : public AbstractPlug
{
  friend PlugIn<T>::PlugIn(Gear* parent, std::string name);
  friend PlugOut<T>::PlugOut(Gear* parent, std::string name);

protected:
  Plug(Gear* parent, eInOut inOut, std::string name, T* type = new T())
    : AbstractPlug(parent, inOut, name, type)
  {
    _type = _defaultType = type;
  }

public:
  virtual ~Plug() 
  {
    delete _type;
  }

  T* type() const { return _type;}

protected:
  T *_type, *_defaultType;
};


template <class T>
class PlugOut : public Plug<T>
{
public:
  PlugOut(Gear* parent, std::string name, T* type = new T())
  : Plug<T>(parent, OUT, name, type)
  {
  }

  virtual ~PlugOut()
  {
  }

  void init() {}
};


template <class T>
class PlugIn : public Plug<T>
{
public:
  PlugIn(Gear* parent, std::string name, T* type = new T())
  : Plug<T>(parent, IN, name, type)
  {
  }

  virtual ~PlugIn()
  {}

  virtual void onConnection(AbstractPlug *plug)
  {
    _type = static_cast<T*>(plug->abstractType());
  }

  virtual void onDisconnection(AbstractPlug *)
  {
    _type = _defaultType;
  }

  void init() {}
};


#endif  //  __PLUG_INCLUDED
