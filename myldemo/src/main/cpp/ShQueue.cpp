//
// Created by Administrator on 2020/10/6.
//

#include "ShQueue.h"

ShQueue::ShQueue(ShStatue *statue) {
    mStatue = statue;
    pthread_cond_init(&cont, NULL);
    pthread_mutex_init(&mutex, NULL);
}

int ShQueue::getQueueSize() {
    int size = -1;
    pthread_mutex_lock(&mutex);
    size = mQueue.size();

    pthread_mutex_unlock(&mutex);
    return size;
}

int ShQueue::putPacket(AVPacket *packet) {
    pthread_mutex_lock(&mutex);
    mQueue.push(packet);
    pthread_cond_signal(&cont);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int ShQueue::getPacket(AVPacket *packet) {
    pthread_mutex_lock(&mutex);
    while (mStatue != NULL && !mStatue->exit) {
        if (mQueue.size() > 0) {
            AVPacket *pPacket = mQueue.front();
            if (av_packet_ref(packet, pPacket) == 0) {
                mQueue.pop();
            }
            av_packet_free(&pPacket);
            av_free(pPacket);
            pPacket = NULL;
            break;
        } else {
            pthread_cond_wait(&cont, &mutex);
        }
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}
