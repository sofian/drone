#ifndef SIGNALTYPE_INCLUDED
#define SIGNALTYPE_INCLUDED

#include "MatrixType.h"

class SignalType : public AbstractType
{
public:
  SignalType() 
  {
    addSubType(_buffer);
    _buffer.resize(Engine::signalInfo().blockSize());    
  }

  virtual ~SignalType(){}
  
  std::string name() const { return "SignalType";}
  QColor color() const { return QColor(249,169,7);}

  MatrixType<float>& buffer() { return _buffer;}

protected:
  MatrixType<float> _buffer;
};

#endif 
