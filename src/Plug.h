#ifndef PLUG_INCLUDED
#define PLUG_INCLUDED

#include <string>
#include <list>
#include <qcolor.h>

#include "Canvas.h"
#include "Engine.h"
#include <iostream> // XXX debug
#include <vector>

enum eInOut {IN, OUT};

class Gear;

class AbstractType
{
public:
  AbstractType() {}
  virtual ~AbstractType() {}

  virtual std::string name() const = 0;
  virtual QColor color() const = 0;

  int nSubTypes() const { return _elements.size(); }
  AbstractType* getSubType(int i) { return _elements[i]; }
  AbstractType* addSubType(AbstractType *elem)
    {
      _elements.push_back(elem);
      return elem;
    }
  
protected:
  std::vector<AbstractType*> _elements;
};

template <class T>
class GenericType: public AbstractType
{
public:
  GenericType() {}

  std::string name() const { return "VideoType"; }
  QColor color() const { return QColor(10,10,200); }

  T& getVal() const { return _val; }
  void setVal() { _val = val; }

protected:
  T _val;
};

template<class T>
class ArrayType: public AbstractType
{
public:
  ArrayType(int size = 0) : _array(0)
    {
      resize(size);
    }

  virtual ~ArrayType()
    { 
      if (_array) 
        free(_array); 
    }
  
  std::string name() const { return "ArrayType"; }
  QColor color() const { return QColor(10,10,20); }

  void resize(int newSize)
    {
      _size = newSize;
      _array = (T*) realloc(_array, _size*sizeof(T));
    }

  int nElements() const { return _size; }
  T getElement(int i) { return _elements[i]; }
  T setElement(int i, T elem)
    {
      _array[i] = elem;
      return elem;
    }

protected:
  T *_array;
  int _size;
};

class VideoType : public AbstractType
{
public:
  VideoType() : _canvas(0) 
    {
      _canvas = new Canvas();
    }

  virtual ~VideoType() { delete _canvas; }

  std::string name() const { return "VideoType"; }
  QColor color() const { return QColor(100,10,20); }

  Canvas* canvas() const { return _canvas; }

private:
  Canvas *_canvas;
};

class SignalType : public AbstractType
{
public:
  SignalType() : _buffer(0) 
    {
      _buffer = (float*) malloc(Engine::signalInfo().blockSize()*sizeof(float));
      memset(_buffer, 0, Engine::signalInfo().blockSize()*sizeof(float));
    }
  virtual ~SignalType() { if (_buffer) free(_buffer); }

  std::string name() const { return "SignalType"; }
  QColor color() const { return QColor(10,100,20); }
  
  float* buffer() const { return _buffer; }

protected:
  float *_buffer;
};

class AbstractPlug  
{
public:
  AbstractPlug(Gear* parent, AbstractType* type, eInOut inOut, std::string name);
  virtual ~AbstractPlug();
    
  virtual void init()=0;
  bool canStartConnection();
  bool canConnect(AbstractPlug *plug, bool onlyTypeCheck=false);
  bool connect(AbstractPlug *plug);
  bool disconnect(AbstractPlug *plug);
  void disconnectAll();

  bool connected() const { return !_connectedPlugs.empty();};

  virtual void onConnection(AbstractPlug*){};//!overloader pour ajouter fonctionnalites APRES une bonne connection
  virtual void onDisconnection(AbstractPlug*){};//!overloader pour ajouter fonctionnalites AVANT deconnection

  AbstractType* abstractType() const { return _abstractType; }
  eInOut inOut() const {return _inOut;};
    
  int connectedPlugs(std::list<AbstractPlug*> &connectedplugs) const;
  AbstractPlug* firstConnectedPlug(){return _connectedPlugs.front();} 
  int nbConnectedPlugs() const {return _connectedPlugs.size();};
  Gear* parent() const {return _parent;};

  std::string fullName() const;
  std::string shortName(int nbChars) const;
  std::string name() const {return _name;};
        
protected:
  std::list<AbstractPlug*> _connectedPlugs;    

private:
  Gear *_parent;
  eInOut _inOut;
  AbstractType *_abstractType;
  std::string _name;
    
};

template <class T>
class PlugOut;
template <class T>
class PlugIn;

template <class T>
class Plug : public AbstractPlug
{
  friend PlugIn<T>::PlugIn(Gear* parent, std::string name);
  friend PlugOut<T>::PlugOut(Gear* parent, std::string name);

private:
  Plug(Gear* parent, eInOut inOut, std::string name)
    : AbstractPlug(parent, new T(), inOut, name)
    {
      _type = static_cast<T*>(abstractType());
      _defaultType = _type;
    }

public:
  virtual ~Plug() 
    {
      delete _type;
    }

  T *type() const { return _type; }

protected:
  T *_type, * _defaultType;
};

template <class T>
class PlugOut : public Plug<T>
{
public:
  PlugOut(Gear* parent, std::string name)
    : Plug<T>(parent, OUT, name)
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
  PlugIn(Gear* parent, std::string name)
    : Plug<T>(parent, IN, name)
    {
    }

  virtual ~PlugIn()
    { }

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


#endif 
