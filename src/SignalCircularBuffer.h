#ifndef SIGNALCIRCULARBUFFER_INCLUDED
#define SIGNALCIRCULARBUFFER_INCLUDED

#include "PlugSignal.h"

class SignalCircularBuffer
{
public:

    SignalCircularBuffer(int size);
    ~SignalCircularBuffer();

    void init(int size);

    void write(Signal_T *buf, int size);
    void read(Signal_T *buf, int &size);

private:

    Signal_T *_Buffer;
    int _ReadPos;
    int _WritePos;
    int _Size;
    int _AvailableData;
    pthread_mutex_t *_Mutex;
    

};

      
#endif
