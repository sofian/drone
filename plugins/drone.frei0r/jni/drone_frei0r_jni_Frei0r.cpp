#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "drone_frei0r_jni_Frei0r.h"
#include "frei0r.h"

//#define __DEBUG

const char* FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE =
	FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE;

struct Frei0rHandle
{
  int (*f0r_init)();
  void (*f0r_deinit)();
//  void (*f0r_get_plugin_info)(f0r_plugin_info_t* pluginInfo);
  void (*f0r_get_param_info)(f0r_param_info_t* info, int param_index);
  f0r_instance_t (*f0r_construct)(unsigned int width, unsigned int height);
  void (*f0r_destruct)(f0r_instance_t instance);
  void (*f0r_set_param_value)(f0r_instance_t instance, f0r_param_t param, int param_index);
  void (*f0r_get_param_value)(f0r_instance_t instance, f0r_param_t param, int param_index);
  void (*f0r_update)(f0r_instance_t instance, double time, const uint32_t* inframe, uint32_t* outframe);
  void (*f0r_update2)(f0r_instance_t instance, double time, const uint32_t* inframe1, 
  						const uint32_t* inframe2, const uint32_t* inframe3, uint32_t* outframe);
  						
  f0r_plugin_info_t pluginInfo;
};

void ThrowFrei0rException(JNIEnv *env, const char* msg, char* error = 0)
{
	/* We don't do much with the exception, except that
	    we print a debug message for it, clear it, and 
	    throw a new exception. */
	 jclass newExcCls;
	 newExcCls = env->FindClass("drone/frei0r/Frei0rException");
	 if (newExcCls == NULL) {
	     /* Unable to find the exception class, give up. */
	     return;
	 }
	 if (error) {
	 	char tmp[1024];
	 	sprintf(tmp, "%s [%s]", msg, error);
		env->ThrowNew(newExcCls, tmp);	
	 }
	 else
		 env->ThrowNew(newExcCls, msg);	
}

Frei0rHandle* GetHandle (JNIEnv *env, jobject obj)
{
	return (Frei0rHandle*) env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "_handle", "J"));
}

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getName
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_drone_frei0r_jni_Frei0r_getName
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
#ifdef __DEBUG
		fprintf(stderr, "Name = %s\n", handle->pluginInfo.name);
#endif
	  	return env->NewStringUTF(handle->pluginInfo.name);
	}
	else
	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return env->NewStringUTF(0);
	}
  }

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getAuthor
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_drone_frei0r_jni_Frei0r_getAuthor
  (JNIEnv *env, jobject obj)
{
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
	  	return env->NewStringUTF(handle->pluginInfo.author);
	}
	else
	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return env->NewStringUTF(0);
	}
}

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getPluginType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getPluginType
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
	  	return (jint) handle->pluginInfo.plugin_type;
	}
	else
	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return (jint)-1;
	}
  }

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getColorModel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getColorModel
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
	  	return (jint) handle->pluginInfo.color_model;
	}
	else
	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return (jint)-1;
	}
  }

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getFrei0rVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getFrei0rVersion
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
	  	return (jint) handle->pluginInfo.frei0r_version;
	}
	else
	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return (jint)-1;
	}
  }

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getMajorVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getMajorVersion
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
	  	return (jint) handle->pluginInfo.major_version;
	}
	else
	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return (jint)-1;
	}
  }

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getMinorVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getMinorVersion
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
	  	return (jint) handle->pluginInfo.minor_version;
	}
	else
	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return (jint)-1;
	}
  }

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    nParams
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_nParams
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
	  	return (jint) handle->pluginInfo.num_params;
	}
	else
	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return (jint)-1;
	}
  }

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getExplanation
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_drone_frei0r_jni_Frei0r_getExplanation
  (JNIEnv *env, jobject obj)
{
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
	  	return env->NewStringUTF(handle->pluginInfo.explanation);
	}
	else
	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return env->NewStringUTF(0);
	}
}

/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    openLibrary
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r__1openLibrary
  (JNIEnv *env, jobject obj, jstring libName)
  {
#ifdef __DEBUG
  	fprintf(stderr, "Opening library.\n");
#endif
  	const char* str = env->GetStringUTFChars(libName, NULL);
	if (str==NULL)
		return;
  	
#ifdef __DEBUG
  	fprintf(stderr, "Opening library handle...\n");
#endif
  	void* handle = dlopen(str, RTLD_LAZY);
  	env->ReleaseStringUTFChars(libName, str);
#ifdef __DEBUG
  	fprintf(stderr, "done: %p.\n", handle);
#endif
	
  	if (!handle)
  	{
  		ThrowFrei0rException(env, "Failed to open native frei0r library.", dlerror());
  		return;
  	}
  	
#ifdef __DEBUG
  	fprintf(stderr, "Creating handle...\n");
#endif
  	Frei0rHandle* frei0rHandle = new Frei0rHandle;
  	memset(frei0rHandle, 0, sizeof(Frei0rHandle));
#ifdef __DEBUG
  	fprintf(stderr, "done.\n");
  	fprintf(stderr, "Dynamic linking...\n");
#endif
  	
	// Now set plugin info.
#ifdef __DEBUG
  	fprintf(stderr, "Binding plugin info...\n");
#endif
	void (*f0r_get_plugin_info)(f0r_plugin_info_t* pluginInfo);
	*(void**) (&f0r_get_plugin_info) = dlsym(handle, "f0r_get_plugin_info");
  	if (*(void**) (&f0r_get_plugin_info)==NULL)
  	{
  		ThrowFrei0rException(env, "Failed to bind f0r_get_plugin_info.", dlerror());
  		return;
  	}
#ifdef __DEBUG
  	fprintf(stderr, "done.\n");
  	fprintf(stderr, "Reading plugin info...\n");
#endif
  	f0r_get_plugin_info(&frei0rHandle->pluginInfo);
#ifdef __DEBUG
  	fprintf(stderr, "done.\n");
#endif

  	*(void**) (&frei0rHandle->f0r_init) = dlsym(handle, "f0r_init");
  	if (*(void**) (&frei0rHandle->f0r_init)==NULL)
  	{
  		ThrowFrei0rException(env, "Failed to bind f0r_init.", dlerror());
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_deinit) = dlsym(handle, "f0r_deinit");
  	if (*(void**) (&frei0rHandle->f0r_deinit)==NULL)
  	{
  		ThrowFrei0rException(env, "Failed to bind f0r_deinit.", dlerror());
  		return;
  	}	

//  	*(void**) (&frei0rHandle->f0r_get_plugin_info) = dlsym(handle, "f0r_get_plugin_info");
//  	if (*(void**) (&frei0rHandle->f0r_get_plugin_info)==NULL)
//  	{
//  		ThrowFrei0rException(env, "Failed to bind f0r_get_plugin_info.", dlerror());
//  		return;
//  	}	

  	*(void**) (&frei0rHandle->f0r_get_param_info) = dlsym(handle, "f0r_get_param_info");
  	if (*(void**) (&frei0rHandle->f0r_get_param_info)==NULL)
  	{
  		ThrowFrei0rException(env, "Failed to bind f0r_get_param_info.", dlerror());
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_construct) = dlsym(handle, "f0r_construct");
  	if (*(void**) (&frei0rHandle->f0r_construct)==NULL)
  	{
  		ThrowFrei0rException(env, "Failed to bind f0r_construct.", dlerror());
  		return;
  	}	
  	
  	*(void**) (&frei0rHandle->f0r_destruct) = dlsym(handle, "f0r_destruct"); 
  	if (*(void**) (&frei0rHandle->f0r_destruct)==NULL)
  	{
  		ThrowFrei0rException(env, "Failed to bind f0r_destruct", dlerror());
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_set_param_value) = dlsym(handle, "f0r_set_param_value");
  	if (*(void**) (&frei0rHandle->f0r_set_param_value)==NULL)
  	{
  		ThrowFrei0rException(env, "Failed to bind f0r_set_param_value.", dlerror());
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_get_param_value) = dlsym(handle, "f0r_get_param_value");
  	if (*(void**) (&frei0rHandle->f0r_get_param_value)==NULL)
  	{
  		ThrowFrei0rException(env, "Failed to bind f0r_get_param_value.", dlerror());
  		return;
  	}	

  	*(void**) (&frei0rHandle->f0r_update) = dlsym(handle, "f0r_update");
  	if (*(void**) (&frei0rHandle->f0r_update)==NULL)
  	{
  		ThrowFrei0rException(env, "Failed to bind f0r_update.", dlerror());
  		return;
  	}	

	*(void**) (&frei0rHandle->f0r_update2) = dlsym(handle, "f0r_update2");
	if ((frei0rHandle->pluginInfo.plugin_type == F0R_PLUGIN_TYPE_MIXER2 ||
			frei0rHandle->pluginInfo.plugin_type == F0R_PLUGIN_TYPE_MIXER3) &&
  		*(void**) (&frei0rHandle->f0r_update2)==NULL)
	  	{
  			ThrowFrei0rException(env, "Failed to bind f0r_update2.", dlerror());
  			return;
	  	}
#ifdef __DEBUG
  	fprintf(stderr, "done.\nSetting up handle (%p)...", frei0rHandle);
#endif

	env->SetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "_handle", "J"), (jlong)frei0rHandle);
#ifdef __DEBUG
  	fprintf(stderr, "done.\n");
#endif
}
  
/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    closeLibrary
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r__1closeLibrary
  (JNIEnv *env, jobject obj)
	{
		Frei0rHandle* handle = GetHandle(env, obj);
		if (handle)
			delete handle;
	}
	
/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    f0r_init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_init
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
  	if (handle)
  		return (jint) handle->f0r_init();
  	else
  	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return (jint)-1;
  	}
  }

/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    f0r_deinit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r_deinit
  (JNIEnv *env, jobject obj)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
		handle->f0r_deinit();
	else
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  }

/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    f0r_construct
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_drone_frei0r_jni_Frei0r_construct
  (JNIEnv *env, jobject obj, jint width, jint height)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	  	return (jlong)handle->f0r_construct((unsigned int)width, (unsigned int)height);
	else
  	{
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  		return (jlong)-1;
  	}
  }

/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    f0r_destruct
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r_destruct
  (JNIEnv *env, jobject obj, jlong inst)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	  	handle->f0r_destruct((f0r_instance_t)inst);
	else
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);
  }

/*
 * Class:     ptolemy_actor_lib_frei0r_Frei0r
 * Method:    f0r_update
 * Signature: (JD[I[I)V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r_update
  (JNIEnv *env, jobject obj, jlong inst, jdouble time, jintArray inArray, jintArray outArray)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
	  	jint* inframe = env->GetIntArrayElements(inArray, 0);
	  	jint* outframe = env->GetIntArrayElements(outArray, 0);
	  	handle->f0r_update((f0r_instance_t)inst, (double)time, (const uint32_t*)inframe, (uint32_t*)outframe);
	  	env->ReleaseIntArrayElements(inArray, inframe, 0);
	  	env->ReleaseIntArrayElements(outArray, outframe, 0);
	}
	else
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);	
  }

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    update2
 * Signature: (JD[I[I[I[I)V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r_update2
  (JNIEnv *env, jobject obj, jlong inst, jdouble time, jintArray inArray1, 
  	jintArray inArray2, jintArray inArray3, jintArray outArray)
  {
	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
		if (handle->pluginInfo.plugin_type == F0R_PLUGIN_TYPE_MIXER2 ||
			handle->pluginInfo.plugin_type == F0R_PLUGIN_TYPE_MIXER3 ||
			*(void**) (&handle->f0r_update2)!=NULL)
		{
		  	jint* inframe1 = env->GetIntArrayElements(inArray1, 0);
		  	jint* inframe2 = env->GetIntArrayElements(inArray2, 0);
		  	jint* inframe3 = env->GetIntArrayElements(inArray3, 0);
		  	jint* outframe = env->GetIntArrayElements(outArray, 0);
		  	handle->f0r_update2((f0r_instance_t)inst, (double)time, (const uint32_t*)inframe1, 
		  							(const uint32_t*)inframe2, (const uint32_t*)inframe3, (uint32_t*)outframe);
		  	env->ReleaseIntArrayElements(inArray1, inframe1, 0);
		  	env->ReleaseIntArrayElements(inArray2, inframe2, 0);
		  	env->ReleaseIntArrayElements(inArray3, inframe3, 0);
		  	env->ReleaseIntArrayElements(outArray, outframe, 0);
		}
	else
		{
			ThrowFrei0rException(env, "Binding problem in calling update2(): f0r_update2() is not binded.");
			return;
		}
	}
	else
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);	
  }

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getParamName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_drone_frei0r_jni_Frei0r_getParamName
  (JNIEnv *env, jobject obj, jint);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getParamType
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getParamType
  (JNIEnv *env, jobject obj, jint);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getParamExplanation
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_drone_frei0r_jni_Frei0r_getParamExplanation
  (JNIEnv *env, jobject obj, jint);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    setParamValue
 * Signature: (JLjava/lang/Object;I)V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r_setParamValue
  (JNIEnv *env, jobject obj, jlong, jobject, jint);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getParamValue
 * Signature: (JI)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_drone_frei0r_jni_Frei0r_getParamValue
  (JNIEnv *env, jobject obj, jlong inst, jint paramIndex)
  {
  	Frei0rHandle* handle = GetHandle(env, obj);
	if (handle)
	{
		if (paramIndex < 0 || paramIndex >= handle->pluginInfo.num_params)
		{
			ThrowFrei0rException(env, "Given index parameter is out of bounds.");
			return NULL;
		}
		else
		{
			f0r_param_info_t paramInfo;
			handle->f0r_get_param_info(&paramInfo, (int)paramIndex);
			jclass newObjClass = NULL;
			jmethodID newObjConstructorID = NULL;
			switch (paramInfo.type)
			{
			case F0R_PARAM_BOOL:
				{
					double boolValue;
					handle->f0r_get_param_value((f0r_instance_t)inst, (f0r_param_t)&boolValue, (int)paramIndex);
					newObjClass = env->FindClass("java/lang/Boolean");
					if (newObjClass == NULL)
					{
						ThrowFrei0rException(env, "Cannot find class java.lang.Boolean.");
						return NULL;
					}
					newObjConstructorID = env->GetMethodId(newObjClass, "<init>", "(Z)");
					if (newObjConstructorID == NULL)
					{
						ThrowFrei0rException(env, "Cannot find constructor Boolean(boolean).");
						return NULL;
					}
					
					return env->NewObject(newObjClass, newObjConstructorID, (jboolean)boolValue);
				}
				break;
			}		
	}
	else
  		ThrowFrei0rException(env, FREI0R_EXCEPTION_MESSAGE_UNINITIALIZED_HANDLE);	
  
  }
