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
