//
// Created by BMW on 2018/8/1.
// ffmpeg解码器
//

#ifndef PLAYMASTER_FFDECODE_H
#define PLAYMASTER_FFDECODE_H


#include "IDecode.h"

class FFDecode :public IDecode{
public:
    virtual bool open(XParameter parameter);

    //发送数据到线程解码
    virtual bool sendPacket(XData data);
    //从线程中获取解码结果
    virtual XData recvFrame();
protected:
    AVCodecContext* codecContext = 0;
    AVFrame* frame = 0;
};


#endif //PLAYMASTER_FFDECODE_H
