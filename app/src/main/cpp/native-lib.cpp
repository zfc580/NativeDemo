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
//Java_com_choufucai_nativedemo_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}


static jstring JNICALL stringFromJNI(JNIEnv *env, jobject obj, jstring javaStr) {

    jboolean isCopy;
    char* str = (char *) env->GetStringUTFChars(javaStr, &isCopy);

    unsigned char b = isCopy;
    char* hello = (char *) "Hello world C++";
    LOGI("str = %s, b = %d",str, isCopy);
    std::string hello1 = NULL;
    hello1.c_str();
    jstring result = env->NewStringUTF(hello);
    return result;
}

static jint JNICALL plus(JNIEnv *env, jobject obj, jint x, jint y){
    jint result = x + y;
    return result;
}

/**
 * 从Java层传递对象参数到Jni层，获取对象值每个成员变量值
 * @param env
 * @param obj
 * @param jBean 传递的参数(除了String,Class,Throwable,其他对象都用jobject表示)
 */
static void JNICALL setParams(JNIEnv *env, jobject obj, jobject jBean){

    //1.找出Java类对象；
    jclass beanClass = env->GetObjectClass(jBean);
    //2.找出Java类成员变量；
    jfieldID idField = env->GetFieldID(beanClass, "id", "I");
    jfieldID nameField = env->GetFieldID(beanClass, "name", "Ljava/lang/String;");
    //3.获取成员变量对应的值；
    jint beanId = env->GetIntField(jBean, idField);
    jstring name = (jstring)env->GetObjectField(jBean, nameField);
    //4.将JNI数据类型转换为C++数据类型
    jboolean isCopy; //C++声明变量即已初始化并附上地址
    const char* nameChar = env->GetStringUTFChars(name, &isCopy);
    unsigned char b = isCopy;
    //如果直接打印name的值会出问题：Fatal signal 11 (SIGSEGV), code 1, fault addr 0x85， Cause: null pointer dereference
    LOGI("JNI setParams beanId =:%d, name = %s, isCopy = %1u\n", beanId, nameChar, b);
    //释放地址
    env->ReleaseStringUTFChars(name, nameChar);
}

static void JNICALL setJniCallback(JNIEnv *env, jobject obj, jobject jcallback){

    const char* resultStr = "Callback Success. ";
    const char *methodName = "onJniCalled";
    const char *methodStrName = "onStrCalled";
    jclass localClass = env->GetObjectClass(jcallback);
    jmethodID bbb = env->GetMethodID(localClass, methodName, "(Ljava/lang/String;)V");
    jmethodID aaaa = env->GetMethodID(localClass, methodStrName, "()Ljava/lang/String;");
    jstring result = env->NewStringUTF(resultStr);
    //通过jni调用java函数，传进去的参数result必须是jni的类型，如jstring,否则报“use of invalid jobject“错误
    env->CallVoidMethod(jcallback, bbb, result);
    jobject returJstr = (jobject) env->CallObjectMethod(jcallback, aaaa);
    jstring jstring1 = (jstring) returJstr;
    jboolean isCopy; //C++声明变量即已初始化并附上地址
    const char* nameChar = env->GetStringUTFChars(jstring1, &isCopy);
    LOGI("JNI setParams returnStr1 = %s\n", nameChar);
}


static const JNINativeMethod gMethods[] = {
        {"stringFromJNI", "(Ljava/lang/String;)Ljava/lang/String;", (jstring *) stringFromJNI},
        {"plus", "(II)I", (jint *)plus},
        {"setParams", "(Lcom/choufucai/nativedemo/Bean;)V", (void *)setParams},
        {"setJniCallback", "(Lcom/choufucai/nativedemo/JniCallback;)V", (void *)setJniCallback}
};


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    //JNIEnv是一个线程相关的结构体, 该结构体代表了Java在本线程的运行环境。通过JNIEnv可以调用到一系列JNI系统函数。
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {//从JavaVM获取JNIEnv，一般使用1.4的版本
        return -1;
    }

    myClass = env->FindClass(kClassName);
    if (myClass == NULL) {
        LOGI("JNI cannot get class:%s\n", kClassName);
        return -1;
    }
    if (env->RegisterNatives(myClass, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        LOGI("JNI register native method failed!\n");
        return -1;
    }
    LOGI("JNI JNI_OnLoad called.");

    return JNI_VERSION_1_4; //这里很重要，必须返回版本，否则加载会失败。
}