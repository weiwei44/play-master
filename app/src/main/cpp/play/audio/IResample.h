//
// Created by BMW on 2018/8/6.
//

#ifndef PLAYMASTER_IRESAMPLE_H
#define PLAYMASTER_IRESAMPLE_H


#include <IObserver.h>
#include "XParameter.h"

class IResample : public IObserver{
public:
    /**
     *
     * @param in  输入参数,从解封装器获取
     * @param out 输出参数，自己配置
     * @return
     */
    virtual bool open(XParameter in,XParameter out = XParameter()) = 0;
    /**
     * 重采样
     * @return
     */
    virtual XData resample(XData data) = 0;

    virtual void update(XData data);

    int outChannels = 2;  //输出通道数
    int outFormat = 1;  //输出样本类型

};


#endif //PLAYMASTER_IRESAMPLE_H
