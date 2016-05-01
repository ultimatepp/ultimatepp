#ifndef _Included_org_upp_AndroidMath_Vector
#define _Included_org_upp_AndroidMath_Vector

#include <jni.h>
#include <map>
#include <vector>
#include <sstream>
#include <string>

#include <AndroidMathUtility/AndroidMathUtility.h>

#include "MemoryManager.h"

#ifdef __cplusplus
extern "C" {
#endif

using AndroidMathUtility::Vector;

MemoryManager<Vector> mm;

/*
 * Class:     org_upp_AndroidMath_Vector
 * Method:    construct
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_upp_AndroidMath_Vector_construct
  (JNIEnv *env, jobject obj, jint size)
{
	mm.Insert(env, obj, Vector(size));
}

/*
 * Class:     org_upp_AndroidMath_Vector
 * Method:    copyConstruct
 * Signature: (Lorg/upp/AndroidMath/Vector;)V
 */
JNIEXPORT void JNICALL Java_org_upp_AndroidMath_Vector_copyConstruct
  (JNIEnv *env, jobject objSrc, jobject objDst)
{
	mm.MakeCopy(env, objSrc, objDst);
}

/*
 * Class:     org_upp_AndroidMath_Vector
 * Method:    destruct
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_upp_AndroidMath_Vector_destroy
  (JNIEnv *env, jobject obj)
{
	mm.Erase(env, obj);
}

/*
 * Class:     org_upp_AndroidMath_Vector
 * Method:    getSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_upp_AndroidMath_Vector_getSize
  (JNIEnv *env, jobject obj)
{
	Vector* vec = mm.Get(env, obj);
	
	return vec->GetSize();
}

/*
 * Class:     org_upp_AndroidMath_Vector
 * Method:    get
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_org_upp_AndroidMath_Vector_get
  (JNIEnv *env, jobject obj, jint id)
{
	Vector* vec = mm.Get(env, obj);
	
	return vec->Get(id);
}

/*
 * Class:     org_upp_AndroidMath_Vector
 * Method:    set
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_org_upp_AndroidMath_Vector_set
  (JNIEnv *env, jobject obj, jint id, jfloat value)
{
	Vector* vec = mm.Get(env, obj);
	vec->Set(id, value);
}

/*
 * Class:     org_upp_AndroidMath_Vector
 * Method:    multipleByScalar
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_org_upp_AndroidMath_Vector_multipleByScalar
  (JNIEnv *env, jobject obj, jfloat scalar)
{
	Vector* vec = mm.Get(env, obj);
	vec->MultipleByScalar(scalar);
}

/*
 * Class:     org_upp_AndroidMath_Vector
 * Method:    toStirng
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_upp_AndroidMath_Vector_toString
  (JNIEnv *env, jobject obj)
{
	Vector* vec = mm.Get(env, obj);
	
	return env->NewStringUTF((vec->ToString()).c_str());
}

#ifdef __cplusplus
}
#endif

#endif
