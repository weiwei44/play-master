//
// Created by BMW on 2018/8/6.
//

#ifndef PLAYMASTER_FFRESAMPLE_H
#define PLAYMASTER_FFRESAMPLE_H


#include "IResample.h"

struct SwrContext;
class FFResample : public IResample{
public:
    virtual bool open(XParameter in,XParameter out = XParameter());

    virtual XData resample(XData data);
protected:
    SwrContext * actx;
};


#endif //PLAYMASTER_FFRESAMPLE_H
