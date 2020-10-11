//
// Created by Administrator on 2020/10/6.
//

#include "ShFfmpeg.h"

ShFfmpeg::ShFfmpeg(const char *url, ShCallBack *pBack, ShStatue *statue, ShQueue *pQueue) {
    this->url = url;
    mStatue = statue;
    mBack = pBack;
    mQueue = pQueue;
}

void *prepareMethod(void *data) {
    ShFfmpeg *shFfmpeg = (ShFfmpeg *) data;
    shFfmpeg->init();
    pthread_exit(&shFfmpeg->prepareThread);
}

void ShFfmpeg::prepare() {

    pthread_create(&prepareThread, NULL, prepareMethod, this);
}

void ShFfmpeg::init() {
    avformat_network_init();
    pAvFormatContext = avformat_alloc_context();
    int ret = avformat_open_input(&pAvFormatContext, url, NULL, NULL);
    if (ret != 0) {
        LOGE("打开文件失败")
        return;
    }
    ret = avformat_find_stream_info(pAvFormatContext, NULL);
    if (ret < 0) {
        LOGE("打开流失败")
        return;
    }

    int rate;
    for (int i = 0; i < pAvFormatContext->nb_streams; ++i) {
        if (pAvFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            mAudio = new ShAudio(mStatue, mQueue,
                                 pAvFormatContext->streams[i]->codecpar->sample_rate);
            mAudio->stream_index = i;
            mAudio->pAVCodecParameters = pAvFormatContext->streams[i]->codecpar;
            break;
        }
    }
    if (mAudio->stream_index == -1) {
        LOGE("寻找流失败")
        return;
    }
    AVCodec *pCodec = avcodec_find_decoder(mAudio->pAVCodecParameters->codec_id);
    if (!pCodec) {
        LOGE("寻找解码器失败")
        return;
    }
    mAudio->pAvCodecContext = avcodec_alloc_context3(pCodec);
    if (!mAudio->pAvCodecContext) {
        LOGE("寻找解码器失败")
        return;
    }
    ret = avcodec_parameters_to_context(mAudio->pAvCodecContext, mAudio->pAVCodecParameters);
    if (ret < 0) {
        LOGE("必要步骤1失败")
        return;
    }
    ret = avcodec_open2(mAudio->pAvCodecContext, pCodec, NULL);
    if (ret != 0) {
        LOGE("必要步骤2失败")
        return;
    }

    mBack->callPrepare(CHILD_THREAD);
}

void ShFfmpeg::start() {
    this->mAudio->play();

    while (mStatue != NULL && !mStatue->exit) {
        AVPacket *pPacket = av_packet_alloc();
        if (av_read_frame(pAvFormatContext, pPacket) == 0) {
            if (pPacket->stream_index == mAudio->stream_index) {
                mQueue->putPacket(pPacket);
                continue;
            } else {
                av_packet_free(&pPacket);
                av_free(pPacket);
            }
        } else {
            av_packet_free(&pPacket);
            av_free(pPacket);
            while (mStatue != NULL && !mStatue->exit) {
                if (mQueue->getQueueSize() > 0) {
                    continue;
                } else {
                    mStatue->exit = true;
                    break;
                }
            }
        }
    }

}
