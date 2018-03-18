#include <jni.h>
#include <string>
#include <android/log.h>

#define  LOG_TAG    "native-dev"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


static jclass myClass;
static const char *kClassName = "com/choufucai/nativedemo/MainActivity";

//extern "C"
//JNIEXPORT jstring
//
//JNICALL
//Java_com_choufucai_nativedemo_MainActivity_stringFromJNI(
//        JNIEnv *env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}


static jstring JNICALL stringFromJNI(JNIEnv *env, jobject obj) {
    std::string hello = "Hello world C++";

    jstring result = env->NewStringUTF(hello.c_str());
    const char *methodName = "onJniCalled";
    const char *className = "com/choufucai/nativedemo/MainActivity";
    jclass localClass = env->FindClass(className);
    jmethodID jmethodID = env->GetMethodID(localClass, methodName, "(Ljava/lang/String;)V");
    //通过jni调用java函数，传进去的参数result必须是jni的类型，如jstring,否则报“use of invalid jobject“错误
    env->CallVoidMethod(obj, jmethodID, result);

    return (env)->NewStringUTF(hello.c_str());
}


static const JNINativeMethod gMethods[] = {
        {"stringFromJNI", "()Ljava/lang/String;", (jstring *) stringFromJNI},
};




JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    //JNIEnv是一个线程相关的结构体, 该结构体代表了Java在本线程的运行环境。通过JNIEnv可以调用到一系列JNI系统函数。
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {//从JavaVM获取JNIEnv，一般使用1.4的版本
        return -1;
    }
    myClass = env->FindClass(kClassName);
    if (myClass == NULL) {
        LOGI("cannot get class:%s\n", kClassName);
        return -1;
    }
    if (env->RegisterNatives(myClass, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        LOGI("register native method failed!\n");
        return -1;
    }
    LOGI("JNI_OnLoad called.");

    return JNI_VERSION_1_4; //这里很重要，必须返回版本，否则加载会失败。
}