#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

// Android 打印 Log
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, "player", FORMAT, ##__VA_ARGS__);

extern "C"
JNIEXPORT void JNICALL
Java_com_rulerbug_myldemo_Demo_play(JNIEnv *env, jobject instance, jstring path_, jobject surface) {
    int result;
    // R1 Java String -> C String
    const char *path = env->GetStringUTFChars(path_, 0);
    avcodec_register_all();
    avformat_network_init();
    AVFormatContext *avFormatContext;
    result = avformat_open_input(&avFormatContext, path, NULL, NULL);
    if (result != 0) {
        LOGE("打开文件失败")
        return;
    }
    result = avformat_find_stream_info(avFormatContext, NULL);
    if (result != 0) {
        LOGE("寻找流失败")
        return;
    }
    int stream_index = -1;
    int64_t duration;
    AVCodecParameters *codepar;
    int rate;
    AVRational rational;
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            stream_index = i;
            codepar = avFormatContext->streams[i]->codecpar;
            duration = avFormatContext->duration / AV_TIME_BASE;
            rate = avFormatContext->streams[i]->codecpar->sample_rate;
            rational = avFormatContext->streams[i]->time_base;
            break;
        }
    }
    AVCodec *pCodec = avcodec_find_decoder(codepar->codec_id);
    if (!pCodec) {
        LOGE("寻找解码器失败")
        return;
    }
    AVCodecContext *pContext = avcodec_alloc_context3(pCodec);
    if (!pContext) {
        LOGE("寻找解码器失败")
        return;
    }
    int context = avcodec_parameters_to_context(pContext, codepar);
    if (context < 0) {
        LOGE(" 失败")
        return;
    }
    int open2 = avcodec_open2(pContext, pCodec, NULL);
    if (open2 != 0) {
        LOGE(" 失败")
        return;
    }
    LOGE("全部成功")
}