//
// Created by BMW on 2018/7/31.
//

#include "FFDemux.h"

FFDemux::FFDemux() {
    av_register_all(); //注册封装器
    avcodec_register_all(); //注册解码器
    avformat_network_init();  //初始化网络
}

bool FFDemux::open(const char *url) {
    int ret = avformat_open_input(&ic,url,0,0);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf, sizeof(buf));
        LOGE("FFDemux open %s , failed!",buf);
        return false;
    }

    //读取文件信息
    ret = avformat_find_stream_info(ic,0);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf, sizeof(buf));
        LOGE("FFDemux avformat_find_stream_info %s , failed!",buf);
        return false;
    }
    this->totalMs = ic->duration/(AV_TIME_BASE/1000);
    LOGD("duration ms = %lld",totalMs);

    getAudioParameter();
    getVideoParameter();
    return true;
}

XParameter FFDemux::getVideoParameter() {
    if(!ic){
        return XParameter();
    }
    //获取视频流
    videoIndex = av_find_best_stream(ic,AVMEDIA_TYPE_VIDEO,-1,-1,0,0);
    if(videoIndex < 0){
        LOGE("not have video stream");
        return XParameter();
    }
    XParameter parameter;
    parameter.codecParameters = ic->streams[videoIndex]->codecpar;
    return parameter;
}

XParameter FFDemux::getAudioParameter() {
    if(!ic){
        return XParameter();
    }

    audioIndex = av_find_best_stream(ic,AVMEDIA_TYPE_AUDIO,-1,-1,0,0);
    if(audioIndex < 0){
        LOGE("not have audio stream");
        return XParameter();
    }
    XParameter parameter;
    parameter.codecParameters = ic->streams[audioIndex]->codecpar;
    return parameter;
}

XData FFDemux::readFrame() {
    if(!ic)return XData();
    XData xData;
    AVPacket *pkt = av_packet_alloc();
    int ret = av_read_frame(ic,pkt);
    if(ret != 0){
        av_packet_free(&pkt);
        return XData();
    }
    xData.data = (unsigned char*)pkt;
    xData.size = pkt->size;
    if(pkt->stream_index == audioIndex){
        xData.isAudio = true;
    }else if(pkt->stream_index == videoIndex){
        xData.isAudio = false;
    }else{
        av_packet_free(&pkt);
        return XData();
    }

    return xData;
}