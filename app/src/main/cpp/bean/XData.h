//
// Created by BMW on 2018/7/31.
//

#ifndef PLAYMASTER_XDATA_H
#define PLAYMASTER_XDATA_H


struct XData {
    unsigned char* data = 0;
    bool isAudio = false;
    int size = 0;
    //清理
    void drop();
};


#endif //PLAYMASTER_XDATA_H
