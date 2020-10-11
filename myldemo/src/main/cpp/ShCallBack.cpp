//
// Created by Administrator on 2020/10/6.
//

#include "ShCallBack.h"

ShCallBack::ShCallBack(JavaVM *vm, JNIEnv *env, jobject obj) {
    this->jvm = vm;
    this->jenv = env;
    this->jobj = obj;
    this->jobj = env->NewGlobalRef(jobj);
    this->jclz = env->GetObjectClass(jobj);
    this->jmidCallPrepare = env->GetMethodID(jclz, "onPrepare", "()V");
}

void ShCallBack::callPrepare(int type) {
    if (type == MAIN_THREAD) {
        jenv->CallVoidMethod(jobj, jmidCallPrepare);
    } else if (type == CHILD_THREAD) {

        jvm->AttachCurrentThread(&jenv, NULL);

        jenv->CallVoidMethod(jobj, jmidCallPrepare);

        jvm->DetachCurrentThread();
    }
}
