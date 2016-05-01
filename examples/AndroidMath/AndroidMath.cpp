#include <string.h>
#include <jni.h>

#include <AndroidMathUtility/AndroidMathUtility.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     org_upp_AndroidMath_AndroidMath
 * Method:    power
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_org_upp_AndroidMath_AndroidMath_power
  (JNIEnv *, jclass, jint number, jint n)
{
      return AndroidMathUtility::Power(number, n);
}

#ifdef __cplusplus
}
#endif
