//
// Created by BMW on 2018/8/2.
//

#ifndef PLAYMASTER_IVIDEOVIEW_H
#define PLAYMASTER_IVIDEOVIEW_H


#include "XData.h"
#include "IObserver.h"
class IVideoView :public IObserver {
public:
    virtual void setRender(void* win) = 0;
    virtual void render(XData xData) = 0;
    virtual void update(XData xData);
};


#endif //PLAYMASTER_IVIDEOVIEW_H
