#include <jni.h>

#ifndef _Included_melco_test_cantool_CanTool
#define _Included_melco_test_cantool_CanTool
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_melco_test_cantool_CanTool_init
  (JNIEnv *, jobject, jstring);

JNIEXPORT jint JNICALL Java_melco_test_cantool_CanTool_sendCommand
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
