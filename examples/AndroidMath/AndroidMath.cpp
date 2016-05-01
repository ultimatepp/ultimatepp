#include <string.h>
#include <jni.h>

#include <AndroidMathUtility/AndroidMathUtility.h>

extern "C" {

	JNIEXPORT jint JNICALL Java_org_upp_AndroidMath_AndroidMath_power(
		JNIEnv *,
		jclass,
		jint number,
		jint n)
	{
	    return AndroidMathUtility::Power(number, n);
	}

} // extern "C"
