#ifndef THREAD_UTIL_INCLUDED
#define THREAD_UTIL_INCLUDED

#include <pthread.h>

class ScopedLock
{
public:  
  ScopedLock(pthread_mutex_t *mutex) :
    _mutex(mutex)
  {
    pthread_mutex_lock(_mutex);
  }
  
  ~ScopedLock()
  {
    pthread_mutex_unlock(_mutex);
  }

private:
  pthread_mutex_t *_mutex;
};


#endif
