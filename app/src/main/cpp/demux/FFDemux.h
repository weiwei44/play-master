//
// Created by BMW on 2018/7/31.
// 解封装
//

#ifndef PLAYMASTER_FFDEMUX_H
#define PLAYMASTER_FFDEMUX_H

#include <string>

#include "IDemux.h"
#include "AndroidLog.h"

extern "C"{
#include "libavformat/avformat.h"
};

class FFDemux : public IDemux {
public:
    FFDemux();
    virtual bool open(const char* url);
    virtual XData readFrame();
    virtual XParameter getVideoParameter();
    virtual XParameter getAudioParameter();

private:
    int64_t totalMs = 0;
    AVFormatContext*  ic = 0;
    int audioIndex = 1;
    int videoIndex = 0;
};


#endif //PLAYMASTER_FFDEMUX_H
