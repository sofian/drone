#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <algorithm>
#include "common.h"
#include "ARToolKitPlus/TrackerMultiMarker.h"
#include "drone_artkp_kernel_TrackerMultiMarker.h"

/*
 * Class:     drone_artkp_kernel_TrackerMultiMarker
 * Method:    init
 * Signature: (Ljava/lang/String;Ljava/lang/String;FF)Z
 */
JNIEXPORT jboolean JNICALL Java_drone_artkp_kernel_TrackerMultiMarker_init
  (JNIEnv *env, jobject obj, jstring nCamParamFile, jstring nMultiFile, jfloat nNearClip, jfloat nFarClip)
{
  ARToolKitPlus::TrackerMultiMarker *tracker = (ARToolKitPlus::TrackerMultiMarker*)getTracker(env, obj);

  if (!tracker)
  {
  	ThrowARTKPException(env, ARTKP_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
	return false;
  }
  
  const char* c_nCamParamFile = env->GetStringUTFChars(nCamParamFile, 0);
  const char* c_nMultiFile = env->GetStringUTFChars(nMultiFile, 0);
  printf("cam file: %s, multi file: %s\n", c_nCamParamFile, c_nMultiFile);
  // init with a NULL logger
  jboolean result = (jboolean)tracker->init(c_nCamParamFile, c_nMultiFile, (ARFloat)nNearClip, (ARFloat)nFarClip);
  printf("init done: %d\n", (int)result);
  env->ReleaseStringUTFChars(nCamParamFile, c_nCamParamFile);
  env->ReleaseStringUTFChars(nMultiFile, c_nMultiFile);
  printf("release done\n");
  return (result);
}

/*
 * Class:     drone_artkp_kernel_TrackerMultiMarker
 * Method:    calc
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_drone_artkp_kernel_TrackerMultiMarker_calc
  (JNIEnv *env, jobject obj, jbyteArray nImage)
{
  ARToolKitPlus::TrackerMultiMarker *tracker = (ARToolKitPlus::TrackerMultiMarker*)getTracker(env, obj);
  
  if (!nImage)
  {
  	env->ThrowNew(env->FindClass("java/lang/NullPointerException"),
  					"Trying to call function with a null image array.");
    return (-1);
  }
  
  if (!tracker)
  {
  	ThrowARTKPException(env, ARTKP_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
	return (-1);
  }

  // Verify if image has the right size.
  jsize arrayLength = env->GetArrayLength(nImage);
  ARToolKitPlus::Camera *cam = tracker->getCamera();
  if (!cam)
  {
  	ThrowARTKPException(env, "Camera not initialized, please load one before calling this function.");
  	return (-1);
  }
  
  jsize camLength = (jsize) (cam->xsize * cam->ysize);
  if (arrayLength != camLength)
  {
    char msg[512];
    sprintf(msg, "Image array has wrong length %d, should be (%dx%d=%d).", arrayLength, cam->xsize, cam->ysize, camLength);
    ThrowARTKPException(env, msg);
    return (-1);
  }
  jbyte *p_nImage = env->GetByteArrayElements(nImage, 0);
  if (p_nImage == 0)
  {
  	env->ThrowNew(env->FindClass("java/lang/NullPointerException"),
  					"Image array is null.");
  }
  jint result = (jint)tracker->calc((unsigned char*)p_nImage);
  env->ReleaseByteArrayElements(nImage, p_nImage, 0);
  return result;
}

/*
 * Class:     drone_artkp_kernel_TrackerMultiMarker
 * Method:    getNumDetectedMarkers
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_artkp_kernel_TrackerMultiMarker_getNumDetectedMarkers
  (JNIEnv *env, jobject obj)
{
  ARToolKitPlus::TrackerMultiMarker *tracker = (ARToolKitPlus::TrackerMultiMarker*)getTracker(env, obj);

  if (!tracker)
  {
  	ThrowARTKPException(env, ARTKP_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
	return (-1);
  }

  return (jint)tracker->getNumDetectedMarkers();
}

/*
 * Class:     drone_artkp_kernel_TrackerMultiMarker
 * Method:    setUseDetectLite
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_drone_artkp_kernel_TrackerMultiMarker_setUseDetectLite
  (JNIEnv *env, jobject obj, jboolean nEnable)
{
  ARToolKitPlus::TrackerMultiMarker *tracker = (ARToolKitPlus::TrackerMultiMarker*)getTracker(env, obj);

  if (!tracker)
  {
  	ThrowARTKPException(env, ARTKP_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
	return;
  }

  tracker->setUseDetectLite((bool)nEnable);
}

/*
 * Class:     drone_artkp_kernel_TrackerMultiMarker
 * Method:    getDetectedMarkers
 * Signature: ([I)V
 */
JNIEXPORT void JNICALL Java_drone_artkp_kernel_TrackerMultiMarker_getDetectedMarkers
  (JNIEnv *env, jobject obj, jintArray nMarkerIDs)
{
  ARToolKitPlus::TrackerMultiMarker *tracker = (ARToolKitPlus::TrackerMultiMarker*)getTracker(env, obj);

  if (!nMarkerIDs)
  {
  	env->ThrowNew(env->FindClass("java/lang/NullPointerException"),
  					"Trying to call function with a null array.");
    return;
  }
	
  if (!tracker)
  {
  	ThrowARTKPException(env, ARTKP_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
	return;
  }

  jsize arrayLength = env->GetArrayLength(nMarkerIDs);
  jsize numDetectedMarkers = tracker->getNumDetectedMarkers();
  if (arrayLength != numDetectedMarkers)
  {
    char msg[512];
    sprintf(msg, "Provided array has wrong length %d, should be %d.", arrayLength, numDetectedMarkers);
    ThrowARTKPException(env, msg);
    return;
  }
  jint *p_nMarkerIDs = env->GetIntArrayElements(nMarkerIDs, 0);
  int *c_nMarkerIDs = 0;
  tracker->getDetectedMarkers(c_nMarkerIDs);
  if (c_nMarkerIDs == 0)
  {
    ThrowARTKPException(env, "In JNI call to getDetectedMarkers(), marker ID array is NULL.");
    return;
  }
  int nMarkers = tracker->getNumDetectedMarkers();
  std::copy(c_nMarkerIDs, c_nMarkerIDs + nMarkers, (int*)p_nMarkerIDs);
  env->ReleaseIntArrayElements(nMarkerIDs, p_nMarkerIDs, 0);
}

/*
 * Class:     drone_artkp_kernel_TrackerMultiMarker
 * Method:    getDetectedMarker
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_drone_artkp_kernel_TrackerMultiMarker_getDetectedMarker
  (JNIEnv *env, jobject obj, jint nWhich)
{
  ARToolKitPlus::TrackerMultiMarker *tracker = (ARToolKitPlus::TrackerMultiMarker*)getTracker(env, obj);

  if (!tracker)
  {
  	ThrowARTKPException(env, ARTKP_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
	return (-1L);
  }

  const ARToolKitPlus::ARMarkerInfo& info = tracker->getDetectedMarker(nWhich);
  return (jlong) &info;
}

/*
 * Class:     drone_artkp_kernel_TrackerMultiMarker
 * Method:    calcOpenGLMatrixFromDetectedMarker
 * Signature: (IFFF[F)F
 */
JNIEXPORT jfloat JNICALL Java_drone_artkp_kernel_TrackerMultiMarker_calcOpenGLMatrixFromDetectedMarker
  (JNIEnv *env, jobject obj, jint nWhich, jfloat nPatternCenterX, jfloat nPatternCenterY, jfloat nPatternSize, jfloatArray nOpenGLMatrix)
{
  ARToolKitPlus::TrackerMultiMarker *tracker = (ARToolKitPlus::TrackerMultiMarker*)getTracker(env, obj);

  if (!tracker)
  {
  	ThrowARTKPException(env, ARTKP_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
	return (-0.0f);
  }

  ARFloat c_nPatternCenter[2] = { (ARFloat) nPatternCenterX, (ARFloat) nPatternCenterY };
  jsize arrayLength = env->GetArrayLength(nOpenGLMatrix);
  if (arrayLength != 16)
  {
    char msg[512];
    sprintf(msg, "Provided array for OpenGL matrix has wrong length %d, should be 16.", arrayLength);
    ThrowARTKPException(env, msg);
  }
  jfloat *p_nOpenGLMatrix = env->GetFloatArrayElements(nOpenGLMatrix, 0);
  const ARToolKitPlus::ARMarkerInfo& info = tracker->getDetectedMarker(nWhich);
  jfloat result = (jfloat)tracker->calcOpenGLMatrixFromMarker((ARToolKitPlus::ARMarkerInfo*)&info, c_nPatternCenter, (ARFloat)nPatternSize, (ARFloat*)p_nOpenGLMatrix);
  env->ReleaseFloatArrayElements(nOpenGLMatrix, p_nOpenGLMatrix, 0);
  return (result);
}
