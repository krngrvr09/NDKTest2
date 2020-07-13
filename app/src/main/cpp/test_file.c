#include <jni.h>

#ifdef __cplusplus
extern "C" {
        JNIEXPORT jstring JNICALL
        Java_com_example_ndktest2_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
                return env->NewStringUTF("Hello from JNI String");
        }
}
#else

        JNIEXPORT jstring JNICALL Java_com_example_ndktest2_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
                return (*env)->NewStringUTF(env, "Hello from JNI String");
        }
#endif
