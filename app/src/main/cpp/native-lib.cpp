#include <jni.h>
#include <string>

#include "FFDemux.h"
#include "FFDecode.h"

class TestObs : public  IObserver{
public:
    virtual void update(XData d){
        //LOGE("data siz = %d",d.size);
    }
};

extern "C"
JNIEXPORT void JNICALL
Java_order_mobile_indoorbuy_com_play_1master_play_XPlay_init(JNIEnv *env, jobject instance,
                                                             jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    TestObs *t = new TestObs();

    IDemux* iDemux = new FFDemux();
    //iDemux->addObserver(t);
    iDemux->open(url);

    IDecode* vdecode = new FFDecode();
    vdecode->open(iDemux->getVideoParameter());

    IDecode* adecode = new FFDecode();
    adecode->open(iDemux->getAudioParameter());

    iDemux->addObserver(vdecode);
    iDemux->addObserver(adecode);

    iDemux->start();
    vdecode->start();
    adecode->start();

//    xSleep(3000);
//    iDemux->stop();
    env->ReleaseStringUTFChars(url_, url);
}