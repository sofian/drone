#include <jni.h>
#include "ARToolKitPlus/Tracker.h"

#ifndef _Included_drone_artkp_kernel_common
#define _Included_drone_artkp_kernel_common
#ifdef __cplusplus
extern "C" {
#endif

const char* ARTKP_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE = 
	"Trying to call a function with an uninitialized handle, please call _createTreackerHandle().";

class MyLogger : public ARToolKitPlus::Logger
{
    void artLog(const char* nStr)
    {
        printf(nStr);
    }
};
  
void ThrowARTKPException(JNIEnv *env, const char* msg)
{
	/* We don't do much with the exception, except that
	    we print a debug message for it, clear it, and 
	    throw a new exception. */
	 jclass newExcCls;
	 newExcCls = env->FindClass("drone/artkp/ARTKPException");
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

  return tracker;
}
#ifdef __cplusplus
}
#endif
#endif

