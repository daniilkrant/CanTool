#include "CanTool_jni.h"
#include "CanSend.hpp"

JNIEXPORT jint JNICALL Java_melco_test_cantool_CanTool_init
  (JNIEnv * env, jobject obj, jstring devname) {
        jboolean isCopy;
        std::string deviceName((env)->GetStringUTFChars(devname, &isCopy));
        return CanSend::getInstance().init(deviceName);
  }

JNIEXPORT jint JNICALL Java_melco_test_cantool_CanTool_sendCommand
  (JNIEnv * env, jobject obj, jstring command) {
        jboolean isCopy;
        std::string canCommand((env)->GetStringUTFChars(command, &isCopy));
        return CanSend::getInstance().sendCommand(canCommand);
  }