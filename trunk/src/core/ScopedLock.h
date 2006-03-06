#ifndef THREAD_UTIL_INCLUDED
#define THREAD_UTIL_INCLUDED

#include <pthread.h>

class MutexLocker
{
public:  
  MutexLocker(pthread_mutex_t *mutex) :
    _mutex(mutex)
  {
    pthread_mutex_lock(_mutex);
  }
  
  ~MutexLocker()
  {
    pthread_mutex_unlock(_mutex);
  }

private:
  pthread_mutex_t *_mutex;
};


#endif
