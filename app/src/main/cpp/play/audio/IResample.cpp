//
// Created by BMW on 2018/8/6.
//

#include "IResample.h"
#include "AndroidLog.h"

void IResample::update(XData data) {
    XData d = this->resample(data);
    if(d.size > 0){
        notify(d);
    }
}