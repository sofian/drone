#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "ARToolKitPlus/Tracker.h"
#include "ptolemy_actor_lib_artkp_kernel_Tracker.h"
#include "common.h"

MyLogger logger;

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    _initTracker
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker__1initTracker
  (JNIEnv *env, jobject obj)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->setLogger(&logger);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    _destroyTrackerHandle
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker__1destroyTrackerHandle
  (JNIEnv *env, jobject obj)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  if (tracker != NULL)
    delete tracker;
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    cleanup
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_cleanup
  (JNIEnv *env, jobject obj)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->cleanup();
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    setPixelFormat
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_setPixelFormat
  (JNIEnv *env, jobject obj, jint nFormat)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->setPixelFormat((ARToolKitPlus::PIXEL_FORMAT)nFormat);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    loadCameraFile
 * Signature: (Ljava/lang/String;FF)Z
 */
JNIEXPORT jboolean JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_loadCameraFile
  (JNIEnv *env, jobject obj, jstring nCamParamFile, jfloat nNearClip, jfloat nFarClip)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  const char* c_nCamParamFile = env->GetStringUTFChars(nCamParamFile, 0);
  tracker->loadCameraFile(c_nCamParamFile, (float)nNearClip, (float)nFarClip);
  env->ReleaseStringUTFChars(nCamParamFile, c_nCamParamFile);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    setLoadUndistLUT
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_setLoadUndistLUT
  (JNIEnv *env, jobject obj, jboolean nSet)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->setLoadUndistLUT((bool)nSet);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    activateBinaryMarker
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_activateBinaryMarker
  (JNIEnv *env, jobject obj, jint nThreshold)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->activateBinaryMarker((int)nThreshold);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    setMarkerMode
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_setMarkerMode
  (JNIEnv *env, jobject obj, jint nMarkerMode)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->setMarkerMode((ARToolKitPlus::MARKER_MODE)nMarkerMode);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    changeCameraSize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_changeCameraSize
  (JNIEnv *env, jobject obj, jint nWidth, jint nHeight)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->changeCameraSize((int)nWidth, (int)nHeight);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    setUndistortionMode
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_setUndistortionMode
  (JNIEnv *env, jobject obj, jint nMode)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->setUndistortionMode((ARToolKitPlus::UNDIST_MODE)nMode);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    setPoseEstimator
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_setPoseEstimator
(JNIEnv *env, jobject obj, jint nMethod)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->setPoseEstimator((ARToolKitPlus::POSE_ESTIMATOR)nMethod);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    setBorderWidth
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_setBorderWidth
  (JNIEnv *env, jobject obj, jfloat nFraction)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->setBorderWidth((float)nFraction);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    setThreshold
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_setThreshold
  (JNIEnv *env, jobject obj, jint nValue)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->setThreshold((int)nValue);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    getThreshold
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_getThreshold
  (JNIEnv *env, jobject obj)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  return (jint) tracker->getThreshold();
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    activateAutoThreshold
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_activateAutoThreshold
  (JNIEnv *env, jobject obj, jboolean nEnable)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->activateAutoThreshold((bool)nEnable);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    isAutoThresholdActivated
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_isAutoThresholdActivated
  (JNIEnv *env, jobject obj)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  return (jboolean) tracker->isAutoThresholdActivated();
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    setNumAutoThresholdRetries
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_setNumAutoThresholdRetries
  (JNIEnv *env, jobject obj, jint nNumRetries)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->setNumAutoThresholdRetries((int)nNumRetries);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    setImageProcessingMode
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_setImageProcessingMode
  (JNIEnv *env, jobject obj, jint nMode)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  tracker->setImageProcessingMode((ARToolKitPlus::IMAGE_PROC_MODE)nMode);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    getModelViewMatrix
 * Signature: ()[F
 */
JNIEXPORT jfloatArray JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_getModelViewMatrix
  (JNIEnv *env, jobject obj)
{
  jfloatArray modelViewMatrix;
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  const ARFloat *c_modelViewMatrix = tracker->getModelViewMatrix();
  modelViewMatrix = env->NewFloatArray(16);
  env->SetFloatArrayRegion(modelViewMatrix, 0, 16, (jfloat*)c_modelViewMatrix);
  return (modelViewMatrix);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    getProjectionMatrix
 * Signature: ()[F
 */
JNIEXPORT jfloatArray JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_getProjectionMatrix
  (JNIEnv *env, jobject obj)
{
  jfloatArray projectionMatrix;
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  const ARFloat *c_projectionMatrix = tracker->getProjectionMatrix();
  projectionMatrix = env->NewFloatArray(16);
  env->SetFloatArrayRegion(projectionMatrix, 0, 16, (jfloat*)c_projectionMatrix);
  return (projectionMatrix);
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    getDescription
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_getDescription
  (JNIEnv *env, jobject obj)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  return env->NewStringUTF(tracker->getDescription());
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    getPixelFormat
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_getPixelFormat
  (JNIEnv *env, jobject obj)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  return (int)tracker->getPixelFormat();
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    getBitsPerPixel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_getBitsPerPixel
  (JNIEnv *env, jobject obj)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  return (int)tracker->getBitsPerPixel();
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    getNumLoadablePatterns
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_getNumLoadablePatterns
  (JNIEnv *env, jobject obj)
{
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  return (int)tracker->getNumLoadablePatterns();
}

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_Tracker
 * Method:    calcOpenGLMatrixFromMarker
 * Signature: (JFFF[F)F
 */
JNIEXPORT jfloat JNICALL Java_ptolemy_actor_lib_artkp_kernel_Tracker_calcOpenGLMatrixFromMarker
  (JNIEnv *env, jobject obj, jlong nMarkerInfo, jfloat nPatternCenterX, jfloat nPatternCenterY, jfloat nPatternSize, jfloatArray nOpenGLMatrix)
{
  ARFloat c_nPatternCenter[2] = { (ARFloat) nPatternCenterX, (ARFloat) nPatternCenterY };
  ARToolKitPlus::Tracker *tracker = getTracker(env, obj);
  jfloat *p_nOpenGLMatrix = env->GetFloatArrayElements(nOpenGLMatrix, 0);	
  jfloat result = (jfloat)tracker->calcOpenGLMatrixFromMarker((ARToolKitPlus::ARMarkerInfo*)nMarkerInfo, c_nPatternCenter, (ARFloat)nPatternSize, (ARFloat*)p_nOpenGLMatrix);
  env->ReleaseFloatArrayElements(nOpenGLMatrix, p_nOpenGLMatrix, 0);
  return (result);
}
