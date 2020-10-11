//
// Created by Administrator on 2020/10/6.
//

#ifndef MYCPP_SHQUEUE_H
#define MYCPP_SHQUEUE_H

#include "queue"
#include "ShStatue.h"
#include "pthread.h"

extern "C" {
#include "libavformat/avformat.h"
};

class ShQueue {

public:
    ShQueue(ShStatue *statue);

public:
    std::queue<AVPacket *> mQueue;
    ShStatue *mStatue;
    pthread_cond_t cont;
    pthread_mutex_t mutex;
public:
    int getQueueSize();

    int putPacket(AVPacket *packet);

    int getPacket(AVPacket *packet);
};


#endif //MYCPP_SHQUEUE_H
