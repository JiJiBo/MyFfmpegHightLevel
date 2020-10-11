#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "ShFfmpeg.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
ShFfmpeg *pFfmpeg;
JavaVM *javaVm;
extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    javaVm = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {

        return result;
    }
    return JNI_VERSION_1_4;

}
extern "C"
JNIEXPORT void JNICALL
Java_com_rulerbug_myldemo_Demo_play(JNIEnv *env, jobject instance, jstring path_, jobject surface) {
    int result;
    // R1 Java String -> C String
    const char *path = env->GetStringUTFChars(path_, 0);
    ShCallBack *pBack = new ShCallBack(javaVm, env, instance);
    ShStatue *pStatue = new ShStatue();
    ShQueue *pQueue = new ShQueue(pStatue);
    pFfmpeg = new ShFfmpeg(path, pBack, pStatue, pQueue);
    pFfmpeg->prepare();
}extern "C"
JNIEXPORT void JNICALL
Java_com_rulerbug_myldemo_Demo_start(JNIEnv *env, jobject thiz) {
    if (pFfmpeg != NULL) {

        pFfmpeg->start();
    }
}