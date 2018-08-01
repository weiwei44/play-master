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
    av_packet_free((AVPacket**)&data);
    data = 0;
    size = 0;
}