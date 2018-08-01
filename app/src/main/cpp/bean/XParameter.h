//
// Created by BMW on 2018/8/1.
//

#ifndef PLAYMASTER_XPARAMETER_H
#define PLAYMASTER_XPARAMETER_H

extern "C" {
#include <libavcodec/avcodec.h>
};

class XParameter {
public:
  AVCodecParameters* codecParameters = 0;
};


#endif //PLAYMASTER_XPARAMETER_H
