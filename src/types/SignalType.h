#ifndef SIGNALTYPE_INCLUDED
#define SIGNALTYPE_INCLUDED

class SignalType : public AbstractType
{
public:
  SignalType() : _buffer(0) 
  {
    _buffer = (float*) malloc(Engine::signalInfo().blockSize()*sizeof(float));
    memset(_buffer, 0, Engine::signalInfo().blockSize()*sizeof(float));
  }
  virtual ~SignalType() { if (_buffer) free(_buffer);}

  std::string name() const { return "SignalType";}
  QColor color() const { return QColor(249,169,7);}

  float* buffer() const { return _buffer;}

protected:
  float *_buffer;
};

#endif 
