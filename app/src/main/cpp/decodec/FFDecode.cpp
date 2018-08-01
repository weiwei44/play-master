//
// Created by BMW on 2018/8/1.
//

#include "FFDecode.h"
#include "AndroidLog.h"

bool FFDecode::open(XParameter parameter) {
    if(!parameter.codecParameters)
        return false;
    AVCodecParameters* p = parameter.codecParameters;
    //1.find decoder
    AVCodec* codec = avcodec_find_decoder(p->codec_id);
    if(!codec){
        LOGE("avcodec_find_decoder %d failed",p->codec_id);
        return false;
    }

    //2.create decoder context
    codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext,p);
    codecContext->thread_count = 8;  //开启8线程解码
    //3.open decoder
    int ret = avcodec_open2(codecContext,0,0); //因为AVCodec已经创建了，所以这里不传
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf, sizeof(buf));
        LOGE("FFDecode avcodec_open2 %s , failed!",buf);
        return false;
    }
    if(codecContext->codec_type == AVMEDIA_TYPE_AUDIO){
        isAudio = true;
    }else if(codecContext->codec_type == AVMEDIA_TYPE_VIDEO){
        isAudio = false;
    }else{
        LOGE("不支持字幕");
        return false;
    }
    LOGD("open decoder success");
    return true;
}

bool FFDecode::sendPacket(XData data) {
    if(data.size <= 0 || !data.data){
        return false;
    }
    if(!codecContext){
        return false;
    }

    int ret = avcodec_send_packet(codecContext,(AVPacket*)data.data);
    return ret == 0;
}

XData FFDecode::recvFrame() {
    if(!codecContext){
        return XData();
    }
    if(!frame){
        frame = av_frame_alloc();
    }
    int ret = avcodec_receive_frame(codecContext,frame);
    if(ret != 0){
        return XData();
    }
    XData d;
    d.data = (unsigned char*)frame;
    if(codecContext->codec_type == AVMEDIA_TYPE_VIDEO) {
        //Y,U,V乘以高度
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
        d.isAudio = false;
    }else if(codecContext->codec_type == AVMEDIA_TYPE_AUDIO){
        //样本字节数 * 单通道样本数 * 通道数
        d.size = av_get_bytes_per_sample((AVSampleFormat)(frame->format)) * frame->nb_samples * 2;
        d.isAudio = true;
    }
     return d;
}