#include <jni.h>
#include "ARToolKitPlus/Tracker.h"

#ifndef _Included_ptolemy_actor_lib_artkp_kernel_common
#define _Included_ptolemy_actor_lib_artkp_kernel_common
#ifdef __cplusplus
extern "C" {
#endif

class MyLogger : public ARToolKitPlus::Logger
{
    void artLog(const char* nStr)
    {
        printf(nStr);
    }
};
  
void ThrowARTKPException(JNIEnv *env, char* msg)
{
	/* We don't do much with the exception, except that
	    we print a debug message for it, clear it, and 
	    throw a new exception. */
	 jclass newExcCls;
	 newExcCls = env->FindClass("ptolemy/actor/lib/artkp/ARTKPException");
	 if (newExcCls == NULL) {
	     /* Unable to find the exception class, give up. */
	     return;
	 }
	 env->ThrowNew(newExcCls, msg);	
}

ARToolKitPlus::Tracker *getTracker(JNIEnv *env, jobject obj)
{
	ARToolKitPlus::Tracker *tracker =
    reinterpret_cast<ARToolKitPlus::Tracker*>(
      env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "_trackerHandle", "J"))
    );

  if (tracker==NULL)
    ThrowARTKPException(env, "Cannot retrieve tracker, make sure you have called the _createTrackerHandle() function.");

  return tracker;
}
#ifdef __cplusplus
}
#endif
#endif

