#ifndef TIMING_INCLUDED
#define TIMING_INCLUDED

typedef double Time_T;

class Timing  
{
public:
    Timing();
    virtual ~Timing();

    static Time_T time();
    static int setProcessFullPriority();
    static void sleep(int msec);

protected:

};

#endif
