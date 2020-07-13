#include <jni.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ndktest2_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF("Hello from JNI LIBS!");
}