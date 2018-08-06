//
// Created by BMW on 2018/8/6.
//

#ifndef PLAYMASTER_SLAUDIOPLAY_H
#define PLAYMASTER_SLAUDIOPLAY_H

#include "IAudioPlay.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SLAudioPlay :public IAudioPlay{
public:
    SLAudioPlay();
    virtual ~SLAudioPlay();
    virtual bool startPlay(XParameter parameter);
    void playCall(void* bufQueue);

protected:
    unsigned char* buf = 0;
    int size = 0;
private:
    SLObjectItf objectItf = NULL;
    SLEngineItf engineItf = NULL;
    SLObjectItf outputMix = NULL;  //混音器
    SLObjectItf play = NULL;  //播放器
    SLPlayItf playItf = NULL;  //播放接口

    SLAndroidSimpleBufferQueueItf pcmQue = NULL; //缓冲队列
    SLresult ret = 0;

    SLEngineItf CreateSL();

};


#endif //PLAYMASTER_SLAUDIOPLAY_H
