#include <iostream>
#include "SignalCircularBuffer.h"



SignalCircularBuffer::SignalCircularBuffer(int size) : _ReadPos(0), _WritePos(0), _Size(0), _AvailableData(0), _Mutex(NULL)
{
  _Mutex = new pthread_mutex_t();
  pthread_mutex_init(_Mutex, NULL);

  init(size);
}

SignalCircularBuffer::~SignalCircularBuffer()
{
  pthread_mutex_destroy(_Mutex);
  delete[] _Buffer;
}

void SignalCircularBuffer::init(int size)
{
  if (_Buffer != NULL)
    delete[] _Buffer;

  _ReadPos=0;
  _WritePos=0;
  _Size=size;
  _AvailableData=0;

  _Buffer = new Signal_T[size];

}

void SignalCircularBuffer::write(Signal_T *buf, int size)
{
  if (size > _Size)
    return;

//    pthread_mutex_lock(_Mutex);

  int len_to_end = _Size - _WritePos;

  if (len_to_end >= size)
    memcpy(&_Buffer[_WritePos], buf, size*sizeof(float));
  else
  {
    memcpy(&_Buffer[_WritePos], buf, len_to_end*sizeof(float));
    memcpy(_Buffer, &buf[len_to_end], (size - len_to_end)*sizeof(float));
  }

  _WritePos = (_WritePos + size) % _Size;    

  std::cout << "_Writepos:" << _WritePos << std::endl;

//    pthread_mutex_unlock(_Mutex);
}

void SignalCircularBuffer::read(Signal_T *buf, int &size)
{
//    pthread_mutex_lock(_Mutex);

  int len_to_end = _Size - _ReadPos;

  if (size > _Size)
    return;

/*     if (size > _AvailableData)           */
/*         available_size = _AvailableData; */
/*     else                                 */
/*         available_size = size;           */

  //cout << "ReadPos:" << _ReadPos << endl;
  //cout << "_AvailableData :" << _AvailableData << endl;

/*     if (available_size > 0)                                                                 */
/*     {                                                                                       */
/*         if (len_to_end >= available_size)                                                   */
/*             memcpy(buf, &_Buffer[_ReadPos], available_size*sizeof(float));                  */
/*         else                                                                                */
/*         {                                                                                   */
/*             memcpy( buf, &_Buffer[_ReadPos], len_to_end*sizeof(float));                     */
/*             memcpy(&buf[len_to_end], _Buffer, (available_size - len_to_end)*sizeof(float)); */
/*         }                                                                                   */
/*     }                                                                                       */
/*                                                                                             */
/*     if (size > available_size)                                                              */
/*         std::fill(&buf[available_size], &buf[size], 0.0f);                                  */


  if (len_to_end >= size)
    memcpy(buf, &_Buffer[_ReadPos], size*sizeof(float));
  else
  {
    memcpy(buf, &_Buffer[_ReadPos], len_to_end*sizeof(float));
    memcpy(&buf[len_to_end], _Buffer, (size - len_to_end)*sizeof(float));
  }

  _ReadPos = (_ReadPos + size) % _Size;    

  std::cout << "_Readpos:" << _ReadPos << std::endl;

//    pthread_mutex_unlock(_Mutex);
}


