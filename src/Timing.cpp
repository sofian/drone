#include "Timing.h"

#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>

const long int MAX_NANO_SLEEP = 999999999;


Timing::Timing()
{
}

Timing::~Timing()
{
}

Time_T Timing::time()
{
  struct timeval tv;

  gettimeofday(&tv, NULL);

  return((tv.tv_sec*1000000) + tv.tv_usec) / 1000.0f;
}

void Timing::sleep(int msec)
{
  struct timespec treq;
  struct timespec trem;
  long int nanosec = msec * 1000000;

  while (nanosec > 0)
  {
    treq.tv_sec = 0;

    if (nanosec > MAX_NANO_SLEEP)
      treq.tv_nsec = MAX_NANO_SLEEP;
    else
      treq.tv_nsec = nanosec;

    trem.tv_sec = 0;
    trem.tv_nsec = 0;

    nanosleep(&treq, &trem);

    nanosec -= MAX_NANO_SLEEP;   

  }  


}

int Timing::setProcessFullPriority()
{
  struct sched_param priority;
  int ecode;

  //Give the current process full priority
  priority.sched_priority = 99;
  ecode = sched_setscheduler(getpid(), SCHED_FIFO, &priority);


  if (ecode)
    std::cout << "cannot set scheduler priority to 99, must be root" << std::endl;

  return ecode;
}

