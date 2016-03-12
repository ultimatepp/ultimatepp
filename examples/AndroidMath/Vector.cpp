#include <jni.h>
#include <map>
#include <vector>
#include <sstream>
#include <string>

#include <AndroidMathUtility/AndroidMathUtility.h>

#include "MemoryManager.h"

using AndroidMathUtility::Vector;

static MemoryManager<Vector> mm;

extern "C" {

	JNIEXPORT void JNICALL Java_org_upp_AndroidMath_Vector_construct(
		JNIEnv *env,
		jobject obj,
		jint size)
	{
		mm.Insert(env, obj, new Vector(size));
	}

	JNIEXPORT void JNICALL Java_org_upp_AndroidMath_Vector_copyConstruct(
		JNIEnv *env,
		jobject jobjThis,
		jobject jobjThat)
	{
		mm.MakeCopy(env, jobjThis, jobjThat);
	}

	JNIEXPORT void JNICALL Java_org_upp_AndroidMath_Vector_nativeFinalize(
		JNIEnv *env,
		jobject obj)
	{
		mm.Erase(env, obj);
	}
	
	JNIEXPORT jint JNICALL Java_org_upp_AndroidMath_Vector_getSize(
		JNIEnv *env,
		jobject obj)
	{
		Vector* vec = mm.Get(env, obj);
		
		return vec->GetSize();
	}

	JNIEXPORT jfloat JNICALL Java_org_upp_AndroidMath_Vector_get(
		JNIEnv *env,
		jobject obj,
		jint id)
	{
		Vector* vec = mm.Get(env, obj);
		
		return vec->Get(id);
	}
	
	JNIEXPORT void JNICALL Java_org_upp_AndroidMath_Vector_set(
		JNIEnv *env,
		jobject obj,
		jint id,
		jfloat value)
	{
		Vector* vec = mm.Get(env, obj);
		vec->Set(id, value);
	}
	
	JNIEXPORT void JNICALL Java_org_upp_AndroidMath_Vector_multipleByScalar(
		JNIEnv *env,
		jobject obj,
		jfloat scalar)
	{
		Vector* vec = mm.Get(env, obj);
		vec->MultipleByScalar(scalar);
	}
	
	JNIEXPORT jstring JNICALL Java_org_upp_AndroidMath_Vector_toString(
		JNIEnv *env,
		jobject obj)
	{
		Vector* vec = mm.Get(env, obj);
		
		return env->NewStringUTF((vec->ToString()).c_str());
	}

} // extern "C"
