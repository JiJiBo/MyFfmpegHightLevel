//
// Created by Administrator on 2020/10/6.
//

#ifndef MYCPP_SHAUDIO_H
#define MYCPP_SHAUDIO_H

#include "ShStatue.h"
#include "ShQueue.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"
};

class ShAudio {
public:

public:
    ShAudio(ShStatue *pStatue, ShQueue *pQueue, int samplerate);

    int stream_index = -1;
    AVCodecParameters *pAVCodecParameters;
    AVCodecContext *pAvCodecContext;
    AVPacket *mAvPacket = NULL;
    AVFrame *mAVFrame = NULL;
    int mRate;
    ShStatue *mStatue;

    void play();

    int getPacketFromQueue();

    ShQueue *mQueue;
    uint8_t *buffer;
    pthread_t initEL;

    int getCurrentSampleRateForOpensles();

    void initELMethod();

    SLObjectItf enginObj;
    SLEngineItf  enginEngin = NULL;

    SLObjectItf  mixObj = NULL;
    SLEnvironmentalReverbItf  mixItf = NULL;
    SLEnvironmentalReverbSettings settings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    SLObjectItf   playObj = NULL;
    SLPlayItf  playItf = NULL;
    SLVolumeItf  volumeItf = NULL;

    SLAndroidSimpleBufferQueueItf slAndroidSimpleBufferQueueItf = NULL;
};


#endif //MYCPP_SHAUDIO_H
