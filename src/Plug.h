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
  enum {ACTIVE,INCATIVE};
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
