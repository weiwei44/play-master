//
// Created by BMW on 2018/8/6.
//

#ifndef PLAYMASTER_IAUDIOPLAY_H
#define PLAYMASTER_IAUDIOPLAY_H

#include "IObserver.h"
#include "XParameter.h"

#include <list>

class IAudioPlay :public IObserver{
public:
    virtual void update(XData data);
    virtual bool startPlay(XParameter parameter) = 0;
    //获取缓冲数据，如果没有，则阻塞
    virtual XData getData();

    int maxFrames = 100;  //最大缓冲

protected:
    std::list<XData> frames;
    std::mutex framesMutex;
};


#endif //PLAYMASTER_IAUDIOPLAY_H
