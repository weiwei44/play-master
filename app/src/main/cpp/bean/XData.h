//
// Created by BMW on 2018/7/31.
//

#ifndef PLAYMASTER_XDATA_H
#define PLAYMASTER_XDATA_H

enum XDataType{
    AVPACKET_TYPE = 0,
    UCHAR_TYPE = 1
};

struct XData {
    int format = 0;
    int type = AVPACKET_TYPE;
    unsigned char* data = 0;
    unsigned char* datas[8] = {0};  //解码数据
    int width = 0;
    int height = 0;
    bool isAudio = false;
    int size = 0;
    bool alloc(int size,const char* data = 0);
    //清理
    void drop();
};


#endif //PLAYMASTER_XDATA_H
