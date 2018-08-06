//
// Created by BMW on 2018/8/6.
//

#include "IAudioPlay.h"
#include "AndroidLog.h"

void IAudioPlay::update(XData data) {
    //压入缓冲队列
    if(data.size <= 0 || !data.data)
        return;

    while (!isExit){
        framesMutex.lock();

        if(frames.size() > maxFrames) {
            //缓冲区满了，阻塞线程,不停的while循环阻塞
            framesMutex.unlock();
            xSleep(1);
            continue;
        }
        frames.push_back(data);
        framesMutex.unlock();
        break;
    }

}

XData IAudioPlay::getData() {
    XData d;
    while (!isExit){
        framesMutex.lock();
        if(!frames.empty()){
            d = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            return d;
        }
        framesMutex.unlock();
        xSleep(1);
    }
    return d;
}