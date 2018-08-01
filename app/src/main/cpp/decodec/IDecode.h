//
// Created by BMW on 2018/8/1.
// 解码接口，支持硬解码
//

#ifndef PLAYMASTER_IDECODE_H
#define PLAYMASTER_IDECODE_H

#include "XParameter.h"
#include "IObserver.h"
#include <list>

class IDecode :public IObserver{
public:
    /**
     * 打开解码器
     * @param parameter
     * @return
     */
    virtual bool open(XParameter parameter) = 0;

    //发送数据到线程解码
    virtual bool sendPacket(XData data) = 0;
    //从线程中获取解码结果
    virtual XData recvFrame() = 0;

    virtual void update(XData data);

    bool isAudio = false;

    //设置最大队列缓冲
    int maxCache = 100;
protected:
    virtual void service();
    //存放缓存
    std::list<XData> pkts;
    std::mutex packsMut;
};


#endif //PLAYMASTER_IDECODE_H
