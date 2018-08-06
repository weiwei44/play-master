//
// Created by BMW on 2018/8/6.
//


#include "FFResample.h"
#include "AndroidLog.h"

extern "C"{
#include <libswresample/swresample.h>
}
bool FFResample::open(XParameter in, XParameter out) {
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(out.channels), AV_SAMPLE_FMT_S16,
                              out.sample_rate, //输出格式
                              av_get_default_channel_layout(in.codecParameters->channels),
                              (AVSampleFormat)(in.codecParameters->format),
                              in.codecParameters->sample_rate,     //输入格式
                              0, 0);
    int ret = swr_init(actx);
    if (ret != 0) {
        LOGE("swr_init failed");
        return false;
    } else {
        LOGE("swr_init success");
    }

    outChannels = out.channels;
    outFormat = AV_SAMPLE_FMT_S16;

    return true;
}

XData FFResample::resample(XData data) {

    if(data.size <= 0 || !data.data){
        return XData();
    }

    if(!actx){
        return XData();
    }

    //输出空间分配
    XData out;
    //通道数*单通道样本数*样本字节大小
    AVFrame* frame = (AVFrame *)(data.data);
    int size = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)(outFormat));
    if(size <= 0) {
        return XData();
    }
    out.alloc(size);

    uint8_t  *outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(actx, outArr, frame->nb_samples,
            (const uint8_t **)(frame->data), frame->nb_samples);

    if(len <= 0){
        out.drop();
        return XData();
    }
    return out;
}