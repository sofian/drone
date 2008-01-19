#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "ARToolKitPlus/Tracker.h"
#include "ARToolKitPlus/TrackerMultiMarker.h"
#include "ARToolKitPlus/TrackerMultiMarkerImpl.h"
#include "drone_artkp_kernel_TrackerMultiMarkerImpl.h"
#include "../common.h"

/*
 * Class:     drone_artkp_kernel_TrackerMultiMarkerImpl
 * Method:    _createTrackerHandle
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_drone_artkp_kernel_TrackerMultiMarkerImpl__1createTrackerHandle__
  (JNIEnv *env, jclass cls)
{
  // return default constructor
  ARToolKitPlus::TrackerMultiMarker *tracker = new ARToolKitPlus::TrackerMultiMarkerImpl<6,6,6, 1, 16>();

  return (jlong) tracker;
}

/*
 * Class:     drone_artkp_kernel_TrackerMultiMarkerImpl
 * Method:    _createTrackerHandle
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_drone_artkp_kernel_TrackerMultiMarkerImpl__1createTrackerHandle__II
  (JNIEnv *env, jclass cls, jint width, jint height)
{
  if (width < 0 || height < 0)
  	ThrowARTKPException(env, "Trying to initialize tracker with negative width or height.");
  
  ARToolKitPlus::TrackerMultiMarker *tracker = new ARToolKitPlus::TrackerMultiMarkerImpl<6,6,6, 1, 16>(width, height);

  return (jlong) tracker;
}
