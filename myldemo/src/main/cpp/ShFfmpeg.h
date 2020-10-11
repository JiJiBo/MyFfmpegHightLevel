//
// Created by Administrator on 2020/10/6.
//

#ifndef MYCPP_SHFFMPEG_H
#define MYCPP_SHFFMPEG_H


#include "AndroidLog.h"
#include "ShStatue.h"
#include "ShCallBack.h"
#include "ShAudio.h"
#include "ShQueue.h"



class ShFfmpeg {
public:
    ShFfmpeg(const char *url, ShCallBack *pBack, ShStatue *statue, ShQueue *pQueue);

public:
    pthread_t prepareThread;
    const char *url;
    ShStatue *mStatue;
    ShCallBack *mBack;
    ShAudio *mAudio;
    ShQueue *mQueue;
    AVFormatContext *pAvFormatContext;
public:
    void prepare();

    void init();

    void start();
};


#endif //MYCPP_SHFFMPEG_H
