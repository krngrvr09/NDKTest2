#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT jstring JNICALL


Java_com_example_ndktest2_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF("Hello from JNI LIBS!");
}
#ifdef __cplusplus
}
#endif
