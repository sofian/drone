#ifndef SIGNALCIRCULARBUFFER_INCLUDED
#define SIGNALCIRCULARBUFFER_INCLUDED

#include "Plug.h"

class SignalCircularBuffer
{
public:

  SignalCircularBuffer(int size);
  ~SignalCircularBuffer();

  void init(int size);

  void write(const float *buf, int size);
  void read(float *buf, int &size);

private:

  float *_buffer;
  int _readPos;
  int _writePos;
  int _size;
  int _availableData;
  pthread_mutex_t *_mutex;


};


#endif
