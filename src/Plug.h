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
  Plug(Gear* parent, eInOut inOut, std::string name, T* type)
    : AbstractPlug(parent, inOut, name, type)
  {    
  }

public:
 
  virtual const T* type() const = 0;
  virtual const T* defaultType() const = 0;
  virtual const T* hintType() const = 0;
  
};


template <class T>
class PlugOut : public Plug<T>
{
public:
  PlugOut(Gear* parent, std::string name, T* type = new T())
  : Plug<T>(parent, OUT, name, type)
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

  void init() {}

private:
   T *_type, *_internalType;
};


template <class T>
class PlugIn : public Plug<T>
{
public:
  PlugIn(Gear* parent, std::string name, T* type = new T())
  : Plug<T>(parent, IN, name, type)
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

  const T* type() const { return _type;}
  const T* defaultType() const { return _internalType; }
  const T* hintType() const { return _internalType; }

protected:
  void setType(const T *type)
  {
    _abstractType = _type = type;
  }

private:
   const T *_type, *_internalType;
};


#endif  //  __PLUG_INCLUDED
