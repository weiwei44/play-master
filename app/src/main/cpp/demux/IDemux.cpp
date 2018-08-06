//
// Created by BMW on 2018/7/31.
//

#include "IDemux.h"
#include "AndroidLog.h"

void IDemux::service() {
    while (!isExit){
        //读取一帧packet
        XData data = readFrame();
        if(data.size <= 0){
            break;
        }else{
            notify(data);
        }
    }
}