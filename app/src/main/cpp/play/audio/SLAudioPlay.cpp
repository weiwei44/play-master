//
// Created by BMW on 2018/8/6.
//

#include "SLAudioPlay.h"
#include "AndroidLog.h"

SLAudioPlay::SLAudioPlay() {
    buf = new unsigned char[1024*1024];
}

SLAudioPlay::~SLAudioPlay() {
    delete buf;
    buf = 0;
}

/**
 * 回调函数
 */
static void pcmCallBack(SLAndroidSimpleBufferQueueItf bf, void *context) {
    SLAudioPlay *audioPlay = (SLAudioPlay *)(context);
    if(!audioPlay){
        LOGE("SLAudioPlay is null");
        return;
    }
    audioPlay->playCall((void *) bf);

}

void SLAudioPlay::playCall(void *bufQueue) {
    if(!bufQueue)
        return;
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf)(bufQueue);
    //从缓冲获取数据，如果没有，会阻塞
    XData d = getData();
    if(d.size <= 0){
        LOGE("getData() = 0");
        return;
    }
    if(!buf)
        return;
    memcpy(buf,d.data,d.size);  //将当前这个xdata数据缓存到buf中
    size = d.size;
    (*bf)->Enqueue(bf, buf, size);
    d.drop();
}

SLEngineItf SLAudioPlay::CreateSL() {
    SLEngineItf engineItf;
    ret = slCreateEngine(&objectItf, 0, 0, 0, 0, 0);
    if (ret != SL_RESULT_SUCCESS) {
        return NULL;
    }

    ret = (*objectItf)->Realize(objectItf,
                                SL_BOOLEAN_FALSE);//SL_BOOLEAN_FALSE表示等待，阻塞线程，等待objectItf创建完毕
    if (ret != SL_RESULT_SUCCESS) {
        return NULL;
    }

    ret = (*objectItf)->GetInterface(objectItf, SL_IID_ENGINE, &engineItf);
    if (ret != SL_RESULT_SUCCESS) {
        return NULL;
    }
    return engineItf;
}

bool SLAudioPlay::startPlay(XParameter parameter) {
    //创建引擎
    engineItf = CreateSL();
    if (engineItf) {
        LOGE("SLEngineItf create success");
    } else {
        LOGE("SLEngineItf create failed");
        return false;
    }

    //创建混音器
    ret = (*engineItf)->CreateOutputMix(engineItf, &outputMix, 0, 0, 0);
    if (ret != SL_RESULT_SUCCESS) {
        LOGE("outputMix create failed");
        return false;
    }
    ret = (*outputMix)->Realize(outputMix, SL_BOOLEAN_FALSE);
    if (ret != SL_RESULT_SUCCESS) {
        LOGE("outputMix Realize failed");
        return false;
    }

    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMix};
    SLDataSink audioSink = {&outmix, 0};  //指定混音器

    //配置PCM信息

    //配置音频缓冲队列，10个长度
    SLDataLocator_AndroidSimpleBufferQueue queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    //音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            (SLuint32)parameter.channels, //声道数
            (SLuint32)parameter.sample_rate * 1000, //采样率
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN  //字节序，小端存储
    };
    SLDataSource dataSource = {&queue, &pcm};

    //创建播放器
    const SLInterfaceID ids[] = {
            SL_IID_BUFFERQUEUE};  //这个参数表示，后面我们需要获取SL_IID_BUFFERQUEUE这个接口，所以需要在这里注册
    const SLboolean req[] = {SL_BOOLEAN_TRUE}; //表示上面对应ids参数接口，开放还是关闭,true表示开放
    ret = (*engineItf)->CreateAudioPlayer(engineItf, &play, &dataSource, &audioSink,
                                          sizeof(ids) / sizeof(SLInterfaceID),  //表示参数的个数
                                          ids,
                                          req);
    if (ret != SL_RESULT_SUCCESS) {
        LOGE("play create failed");
        return false;
    }
    ret = (*play)->Realize(play, SL_BOOLEAN_FALSE);
    if (ret != SL_RESULT_SUCCESS) {
        LOGE("play Realize failed");
        return false;
    }

    //获取play接口
    ret = (*play)->GetInterface(play, SL_IID_PLAY, &playItf);
    if (ret != SL_RESULT_SUCCESS) {
        LOGE("playItf GetInterface  SL_IID_PLAY failed");
        return false;
    }

    //获取播放缓冲队列
    ret = (*play)->GetInterface(play, SL_IID_BUFFERQUEUE, &pcmQue);
    if (ret != SL_RESULT_SUCCESS) {
        LOGE("pcmQue GetInterface  SL_IID_BUFFERQUEUE failed");
        return false;
    }

    //设置缓冲接口的回调函数
    (*pcmQue)->RegisterCallback(pcmQue,
                                pcmCallBack,  //回调函数
                                this); //回调函数的参数
    //设置状态,播放状态
    (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PLAYING);

    //启动队列，给队列插入播放数据,就插入一个空数据，把队列调用起来就OK了
    (*pcmQue)->Enqueue(pcmQue, "", 1);
    return true;
}