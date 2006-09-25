#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "drone_plugins_frei0r_actors_Frei0r.h"
#include "frei0r.h"

struct Frei0rHandle
{
  int (*f0r_init)();
  void (*f0r_deinit)();
  void (*f0r_get_plugin_info)(f0r_plugin_info_t* pluginInfo);
  void (*f0r_get_param_info)(f0r_param_info_t* info, int param_index);
  f0r_instance_t (*f0r_construct)(unsigned int width, unsigned int height);
  void (*f0r_destruct)(f0r_instance_t instance);
  void (*f0r_set_param_value)(f0r_instance_t instance, f0r_param_t param, int param_index);
  void (*f0r_get_param_value)(f0r_instance_t instance, f0r_param_t param, int param_index);
  void (*f0r_update)(f0r_instance_t instance, double time, const uint32_t* inframe, uint32_t* outframe);
};

void ThrowFrei0rException(JNIEnv *env, char* msg)
{
	/* We don't do much with the exception, except that
	    we print a debug message for it, clear it, and 
	    throw a new exception. */
	 jclass newExcCls;
	 newExcCls = env->FindClass("drone/plugins/frei0r/Frei0rException");
	 if (newExcCls == NULL) {
	     /* Unable to find the exception class, give up. */
	     return;
	 }
	 env->ThrowNew(newExcCls, msg);	
}

/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    openLibrary
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_drone_plugins_frei0r_actors_Frei0r_openLibrary
  (JNIEnv *env, jobject obj, jstring libName)
  {
  	char *error;

  	const char* str = env->GetStringUTFChars(libName, NULL);
	if (str==NULL)
		return;  	
  	
  	void* handle = dlopen(str, RTLD_LAZY);
  	env->ReleaseStringUTFChars(libName, str);
	
  	if (!handle)
  	{
  		ThrowFrei0rException(env, "fail to open native frei0r lib");
  		return;
  	}	
  	
  	Frei0rHandle* frei0rHandle = new Frei0rHandle;
  	memset(frei0rHandle, 0, sizeof(Frei0rHandle));
  	
  	*(void**) (&frei0rHandle->f0r_init) = dlsym(handle, "f0r_init");
  	if (*(void**) (&frei0rHandle->f0r_init)==NULL)
  	{
 		if ((error = dlerror()) != NULL)  {
        	fprintf (stderr, "%s\n", error); 		
 		}
  		ThrowFrei0rException(env, "fail to bind f0r_init");
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_deinit) = dlsym(handle, "f0r_deinit");
  	if (*(void**) (&frei0rHandle->f0r_deinit)==NULL)
  	{
  		ThrowFrei0rException(env, "fail to bind f0r_deinit");
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_get_plugin_info) = dlsym(handle, "f0r_get_plugin_info");
  	if (*(void**) (&frei0rHandle->f0r_get_plugin_info)==NULL)
  	{
  		ThrowFrei0rException(env, "fail to bind f0r_get_plugin_info");
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_get_param_info) = dlsym(handle, "f0r_get_param_info");
  	if (*(void**) (&frei0rHandle->f0r_get_param_info)==NULL)
  	{
  		ThrowFrei0rException(env, "fail to bind f0r_get_param_info");
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_construct) = dlsym(handle, "f0r_construct");
  	if (*(void**) (&frei0rHandle->f0r_construct)==NULL)
  	{
  		ThrowFrei0rException(env, "fail to bind f0r_construct");
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_destruct) = dlsym(handle, "f0r_destruct"); 
  	if (*(void**) (&frei0rHandle->f0r_destruct)==NULL)
  	{
  		ThrowFrei0rException(env, "fail to bind f0r_destruct");
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_set_param_value) = dlsym(handle, "f0r_set_param_value");
  	if (*(void**) (&frei0rHandle->f0r_set_param_value)==NULL)
  	{
  		ThrowFrei0rException(env, "fail to bind f0r_set_param_value");
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_get_param_value) = dlsym(handle, "f0r_get_param_value");
  	if (*(void**) (&frei0rHandle->f0r_get_param_value)==NULL)
  	{
  		ThrowFrei0rException(env, "fail to bind f0r_get_param_value");
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_update) = dlsym(handle, "f0r_update");
  	if (*(void**) (&frei0rHandle->f0r_update)==NULL)
  	{
  		ThrowFrei0rException(env, "fail to bind f0r_update");
  		return;
  	}	

	env->SetIntField(obj, env->GetFieldID(env->GetObjectClass(obj), "_handle", "I"), (jint)frei0rHandle);
}
  
/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    closeLibrary
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_drone_plugins_frei0r_actors_Frei0r_closeLibrary
  (JNIEnv *env, jobject obj)
	{
		Frei0rHandle* handle = (Frei0rHandle*) env->GetIntField(obj, env->GetFieldID(env->GetObjectClass(obj), "_handle", "I"));
		delete handle;
	}
	
/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    f0r_init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_plugins_frei0r_actors_Frei0r_f0r_1init
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = (Frei0rHandle*) env->GetIntField(obj, env->GetFieldID(env->GetObjectClass(obj), "_handle", "I"));
  	if (handle==NULL)
  		return (jint)-1;
  	return (jint) handle->f0r_init();
  }

/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    f0r_deinit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_drone_plugins_frei0r_actors_Frei0r_f0r_1deinit
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = (Frei0rHandle*) env->GetIntField(obj, env->GetFieldID(env->GetObjectClass(obj), "_handle", "I"));

  	handle->f0r_deinit();
  }

/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    f0r_construct
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_drone_plugins_frei0r_actors_Frei0r_f0r_1construct
  (JNIEnv *env, jobject obj, jint width, jint height)
  {
	Frei0rHandle* handle = (Frei0rHandle*) env->GetIntField(obj, env->GetFieldID(env->GetObjectClass(obj), "_handle", "I"));
  	return (jint)handle->f0r_construct((unsigned int)width, (unsigned int)height);
  }

/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    f0r_destruct
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_drone_plugins_frei0r_actors_Frei0r_f0r_1destruct
  (JNIEnv *env, jobject obj, jint inst)
  {
	Frei0rHandle* handle = (Frei0rHandle*) env->GetIntField(obj, env->GetFieldID(env->GetObjectClass(obj), "_handle", "I"));
  	handle->f0r_destruct((f0r_instance_t)inst);
  }

/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    f0r_update
 * Signature: (ID[I[I)V
 */
JNIEXPORT void JNICALL Java_drone_plugins_frei0r_actors_Frei0r_f0r_1update
  (JNIEnv *env, jobject obj, jint inst, jdouble time, jintArray inArray, jintArray outArray)
  {
	Frei0rHandle* handle = (Frei0rHandle*) env->GetIntField(obj, env->GetFieldID(env->GetObjectClass(obj), "_handle", "I"));
  	jint* inframe = env->GetIntArrayElements(inArray, 0);
  	jint* outframe = env->GetIntArrayElements(outArray, 0);
  	handle->f0r_update((f0r_instance_t)inst, (double)time, (const uint32_t*)inframe, (uint32_t*)outframe);
  	env->ReleaseIntArrayElements(inArray, inframe, 0);
  	env->ReleaseIntArrayElements(outArray, outframe, 0);
  }

