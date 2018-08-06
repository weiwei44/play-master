//
// Created by BMW on 2018/7/31.
//

extern "C" {
#include <libavcodec/avcodec.h>
}
#include "XData.h"

/**
 * 空间清理
 */
void XData::drop() {
    if(!data)return;
    if(type == AVPACKET_TYPE){
        av_packet_free((AVPacket**)&data);
    }else{
        delete data;
    }

    data = 0;
    size = 0;
}

//分配空间
bool XData::alloc(int size, const char *d) {
    drop();
    type = UCHAR_TYPE;
    if(size <= 0){
        return false;
    }
    this->data = new unsigned char[size];
    if(!this->data) { //空间分配失败
        return false;
    }
    if(d){  //d不为null,就把数据赋值给data
        memcpy(this->data,d,size);
    }
    this->size = size;
    return true;
}