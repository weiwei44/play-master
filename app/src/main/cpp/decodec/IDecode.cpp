//
// Created by BMW on 2018/8/1.
//

#include "IDecode.h"
#include "AndroidLog.h"

void IDecode::service() {

    while (!isExit) {
        packsMut.lock();

        if(pkts.empty()){
            packsMut.unlock();
            xSleep(1);
            continue;
        }

        //从队列中取出packet
        XData data = pkts.front();
        pkts.pop_front();


        //解码
        if(this->sendPacket(data)){
            //一个数据包可能多次解码
            while (!isExit){
                XData frame = this->recvFrame();
                if(!frame.data)break;  //读不到数据就跳出循环
                if(frame.isAudio){
                    //LOGE("音频帧: %d",frame.size);
                } else{
                    //LOGE("视频帧: %d",frame.size);
                }
                this->notify(frame);
            }
        }
        data.drop();
        packsMut.unlock();
    }

}

void IDecode::update(XData data) {
    if(data.isAudio != isAudio){
        return;
    }

    while (!isExit){
        packsMut.lock();

        if(pkts.size() < maxCache){
            pkts.push_back(data);
            packsMut.unlock();
            break;
        }

        //如果大于最大缓冲,就阻塞
        packsMut.unlock();
        xSleep(1);

    }


}