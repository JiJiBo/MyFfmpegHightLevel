//
// Created by Administrator on 2020/10/6.
//

#ifndef MYCPP_SHCALLBACK_H
#define MYCPP_SHCALLBACK_H
#define MAIN_THREAD 0
#define CHILD_THREAD 1

#include "AndroidLog.h"
#include "jni.h"

class ShCallBack {
public:
    ShCallBack(JavaVM *vm,JNIEnv *env,jobject obj);

public:
    JavaVM *jvm;
    JNIEnv *jenv;
    jobject jobj;
    jclass jclz;
    jmethodID jmidCallPrepare;
public:
    void callPrepare(int type);
};


#endif //MYCPP_SHCALLBACK_H
