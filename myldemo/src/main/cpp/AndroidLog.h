//
// Created by Administrator on 2020/10/6.
//

#ifndef MYCPP_ANDROIDLOG_H
#define MYCPP_ANDROIDLOG_H

#endif //MYCPP_ANDROIDLOG_H
#include <android/log.h>
// Android 打印 Log
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, "player", FORMAT, ##__VA_ARGS__);