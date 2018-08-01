//
// Created by BMW on 2018/7/31.
// 解封装接口
//

#ifndef PLAYMASTER_IDEMUX_H
#define PLAYMASTER_IDEMUX_H

#include "XData.h"
#include "IObserver.h"
#include "XParameter.h"

class IDemux :public IObserver{
public:
    //打开文件
    virtual bool open(const char* url) = 0;

    //获取视频解码器参数
    virtual XParameter getVideoParameter() = 0;

    virtual XParameter getAudioParameter() = 0;

    //读取一帧数据，数据记得清理
    virtual XData readFrame() = 0;

protected:
    virtual void service();
};


#endif //PLAYMASTER_IDEMUX_H
