/* SignalCircularBuffer.cpp
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <iostream>
#include "SignalCircularBuffer.h"



SignalCircularBuffer::SignalCircularBuffer(int size) : _readPos(0), _writePos(0), _size(0), _availableData(0), _mutex(NULL)
{
  _mutex = new pthread_mutex_t();
  pthread_mutex_init(_mutex, NULL);

  init(size);
}

SignalCircularBuffer::~SignalCircularBuffer()
{
  pthread_mutex_destroy(_mutex);
  delete[] _buffer;
}

void SignalCircularBuffer::init(int size)
{
  if (_buffer != NULL)
    delete[] _buffer;

  _readPos=0;
  _writePos=0;
  _size=size;
  _availableData=0;

  _buffer = new float[size];

}

void SignalCircularBuffer::write(const float *buf, int size)
{
  if (size > _size)
    return;

//    pthread_mutex_lock(_mutex);

  int len_to_end = _size - _writePos;

  if (len_to_end >= size)
    memcpy(&_buffer[_writePos], buf, size*sizeof(float));
  else
  {
    memcpy(&_buffer[_writePos], buf, len_to_end*sizeof(float));
    memcpy(_buffer, &buf[len_to_end], (size - len_to_end)*sizeof(float));
  }

  _writePos = (_writePos + size) % _size;    

  std::cout << "_writepos:" << _writePos << std::endl;

//    pthread_mutex_unlock(_mutex);
}

void SignalCircularBuffer::read(float *buf, int &size)
{
//    pthread_mutex_lock(_mutex);

  int len_to_end = _size - _readPos;

  if (size > _size)
    return;

/*     if (size > _availableData)           */
/*         available_size = _availableData; */
/*     else                                 */
/*         available_size = size;           */

  //cout << "ReadPos:" << _readPos << endl;
  //cout << "_availableData :" << _availableData << endl;

/*     if (available_size > 0)                                                                 */
/*     {                                                                                       */
/*         if (len_to_end >= available_size)                                                   */
/*             memcpy(buf, &_buffer[_readPos], available_size*sizeof(float));                  */
/*         else                                                                                */
/*         {                                                                                   */
/*             memcpy( buf, &_buffer[_readPos], len_to_end*sizeof(float));                     */
/*             memcpy(&buf[len_to_end], _buffer, (available_size - len_to_end)*sizeof(float)); */
/*         }                                                                                   */
/*     }                                                                                       */
/*                                                                                             */
/*     if (size > available_size)                                                              */
/*         std::fill(&buf[available_size], &buf[size], 0.0f);                                  */


  if (len_to_end >= size)
    memcpy(buf, &_buffer[_readPos], size*sizeof(float));
  else
  {
    memcpy(buf, &_buffer[_readPos], len_to_end*sizeof(float));
    memcpy(&buf[len_to_end], _buffer, (size - len_to_end)*sizeof(float));
  }

  _readPos = (_readPos + size) % _size;    

  std::cout << "_readpos:" << _readPos << std::endl;

//    pthread_mutex_unlock(_mutex);
}


