//
// Created by Administrator on 2020/10/6.
//

#include "ShAudio.h"

void *initMethod(void *data) {
    ShAudio *audio = (ShAudio *) data;
    audio->initELMethod();
    pthread_exit(&audio->initEL);
}

void ShAudio::play() {
    pthread_create(&initEL, NULL, initMethod, this);
}

int ShAudio::getPacketFromQueue() {
    int size;
    while (mStatue != NULL && !mStatue->exit) {
        if (mQueue->getQueueSize() == 0) {
            if (mQueue->mStatue->load) {
                mQueue->mStatue->load= true;
                continue;
            }
        } else{
            if (mQueue->mStatue->load) {
                mQueue->mStatue->load= false;
            }
        }
        mAvPacket = av_packet_alloc();
        if (mQueue->getPacket(mAvPacket) != 0) {
            av_packet_free(&mAvPacket);
            av_free(mAvPacket);
            mAvPacket = NULL;
            continue;
        }
        int ret;
        ret = avcodec_send_packet(pAvCodecContext, mAvPacket);
        if (ret != 0) {
            av_packet_free(&mAvPacket);
            av_free(mAvPacket);
            mAvPacket = NULL;
            continue;
        }
        mAVFrame = av_frame_alloc();
        ret = avcodec_receive_frame(pAvCodecContext, mAVFrame);
        if (ret != 0) {
            av_packet_free(&mAvPacket);
            av_free(mAvPacket);
            mAvPacket = NULL;
            av_frame_free(&mAVFrame);
            av_free(mAVFrame);
            mAVFrame = NULL;
            continue;
        }
        if (mAVFrame->channels == 0 && mAVFrame->channel_layout > 0) {
            mAVFrame->channels = av_get_channel_layout_nb_channels(mAVFrame->channel_layout);
        } else if (mAVFrame->channels > 0 && mAVFrame->channel_layout == 0) {
            mAVFrame->channel_layout = av_get_default_channel_layout(mAVFrame->channels);
        }

        SwrContext *swrContext;
        swrContext = swr_alloc_set_opts(
                NULL,
                AV_CH_LAYOUT_STEREO,
                AV_SAMPLE_FMT_S16,
                mAVFrame->sample_rate,
                mAVFrame->channel_layout,
                (AVSampleFormat) (mAVFrame->format),
                mAVFrame->sample_rate,
                NULL,
                NULL
        );
        if (!swrContext || swr_init(swrContext) < 0) {
            av_packet_free(&mAvPacket);
            av_free(mAvPacket);
            mAvPacket = NULL;
            av_frame_free(&mAVFrame);
            av_free(mAVFrame);
            mAVFrame = NULL;
            swr_free(&swrContext);
            continue;
        }
        int nb = swr_convert(swrContext,
                             &buffer,
                             mAVFrame->nb_samples,
                             (const uint8_t **) (mAVFrame->data),
                             mAVFrame->nb_samples
        );
        int channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
        size = nb * channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
        av_packet_free(&mAvPacket);
        av_free(mAvPacket);
        mAvPacket = NULL;
        av_frame_free(&mAVFrame);
        av_free(mAVFrame);
        mAVFrame = NULL;
        swr_free(&swrContext);
        break;
    }
    return size;
}

ShAudio::ShAudio(ShStatue *pStatue, ShQueue *pQueue, int samplerate) {
    mStatue = pStatue;
    mQueue = pQueue;
    mRate = samplerate;
    buffer = (uint8_t *) (malloc(mRate * 2 * 2));
}

int ShAudio::getCurrentSampleRateForOpensles() {
    int rate = 0;
    switch (mRate) {
        case 8000:
            rate = SL_SAMPLINGRATE_8;
            break;
        case 11025:
            rate = SL_SAMPLINGRATE_11_025;
            break;
        case 12000:
            rate = SL_SAMPLINGRATE_12;
            break;
        case 16000:
            rate = SL_SAMPLINGRATE_16;
            break;
        case 22050:
            rate = SL_SAMPLINGRATE_22_05;
            break;
        case 24000:
            rate = SL_SAMPLINGRATE_24;
            break;
        case 32000:
            rate = SL_SAMPLINGRATE_32;
            break;
        case 44100:
            rate = SL_SAMPLINGRATE_44_1;
            break;
        case 48000:
            rate = SL_SAMPLINGRATE_48;
            break;
        case 64000:
            rate = SL_SAMPLINGRATE_64;
            break;
        case 88200:
            rate = SL_SAMPLINGRATE_88_2;
            break;
        case 96000:
            rate = SL_SAMPLINGRATE_96;
            break;
        case 192000:
            rate = SL_SAMPLINGRATE_192;
            break;
        default:
            rate = SL_SAMPLINGRATE_44_1;
    }
    return rate;
}

void bufferCallback(SLAndroidSimpleBufferQueueItf queueItf, void *data) {
    ShAudio *audio = (ShAudio *) (data);
    int size = audio->getPacketFromQueue();
    if (size > 0) {
        (*audio->slAndroidSimpleBufferQueueItf)->Enqueue(audio->slAndroidSimpleBufferQueueItf,
                                                         audio->buffer, size);
    }
}

void ShAudio::initELMethod() {
    SLresult sLresult;
    sLresult = slCreateEngine(&enginObj, 0, 0, 0, 0, 0);
    sLresult = (*enginObj)->Realize(enginObj, SL_BOOLEAN_FALSE);
    sLresult = (*enginObj)->GetInterface(enginObj, SL_IID_ENGINE, &enginEngin);

    SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    SLboolean mreq[1] = {SL_BOOLEAN_FALSE};

    sLresult = (*enginEngin)->CreateOutputMix(enginEngin, &mixObj, 1, mids, mreq);
    sLresult = (*mixObj)->Realize(mixObj, SL_BOOLEAN_FALSE);
    sLresult = (*mixObj)->GetInterface(mixObj, SL_IID_ENVIRONMENTALREVERB, &mixItf);
    if (sLresult == SL_RESULT_SUCCESS) {
        sLresult = (*mixItf)->SetEnvironmentalReverbProperties(mixItf, &settings);
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, mixObj};
    SLDataLocator_AndroidSimpleBufferQueue androidSimpleBufferQueue = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};

    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2,
            (SLuint32) (getCurrentSampleRateForOpensles()),
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource slDataSource = {&androidSimpleBufferQueue, &pcm};
    SLDataSink slDataSink = {&outputMix, 0};

    SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    SLboolean req[1] = {SL_BOOLEAN_TRUE};
    sLresult = (*enginEngin)->CreateAudioPlayer(enginEngin, &playObj, &slDataSource, &slDataSink, 1,
                                                ids, req);
    sLresult = (*playObj)->Realize(playObj, SL_BOOLEAN_FALSE);
    sLresult = (*playObj)->GetInterface(playObj, SL_IID_PLAY, &playItf);
    sLresult = (*playObj)->GetInterface(playObj, SL_IID_BUFFERQUEUE,
                                        &slAndroidSimpleBufferQueueItf);
    sLresult = (*playObj)->GetInterface(playObj, SL_IID_VOLUME, &volumeItf);
    sLresult = (*slAndroidSimpleBufferQueueItf)->RegisterCallback(slAndroidSimpleBufferQueueItf,
                                                                  bufferCallback, this);
    sLresult = (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PLAYING);
    bufferCallback(slAndroidSimpleBufferQueueItf, this);
}
