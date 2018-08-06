#include <jni.h>
#include <string>

#include "FFDemux.h"
#include "FFDecode.h"
#include "FFResample.h"
#include "SLAudioPlay.h"

#include <android/native_window_jni.h>
#include <XShader.h>

#include "GLVideoView.h"


IVideoView* view = NULL;
IDemux* iDemux = NULL;
IDecode* vdecode = NULL;
IDecode* adecode = NULL;
IResample* resample;
class TestObs : public  IObserver{
public:
    virtual void update(XData d){
        //LOGE("data siz = %d",d.size);
    }
};

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *res) {
    FFDecode::initHard(vm);
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_order_mobile_indoorbuy_com_play_1master_play_XPlay_init(JNIEnv *env, jobject instance,
                                                             jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    TestObs *t = new TestObs();

    iDemux = new FFDemux();
    //iDemux->addObserver(t);
    iDemux->open(url);

    vdecode = new FFDecode();
    vdecode->open(iDemux->getVideoParameter(),true);

    adecode = new FFDecode();
    adecode->open(iDemux->getAudioParameter());

    iDemux->addObserver(vdecode);
    iDemux->addObserver(adecode);

    view = new GLVideoView();
    vdecode->addObserver(view);

    resample = new FFResample();
    XParameter out = iDemux->getAudioParameter();

    resample->open(iDemux->getAudioParameter(),out);
    adecode->addObserver(resample);

    IAudioPlay* audioPlay = new SLAudioPlay();
    audioPlay->startPlay(out);
    resample->addObserver(audioPlay);

    iDemux->start();
    vdecode->start();
    adecode->start();

//    xSleep(3000);
//    iDemux->stop();
    env->ReleaseStringUTFChars(url_, url);
}extern "C"
JNIEXPORT void JNICALL
Java_order_mobile_indoorbuy_com_play_1master_play_XPlay_initView(JNIEnv *env, jobject instance,
                                                                 jobject holder) {
    ANativeWindow* win = ANativeWindow_fromSurface(env,holder);
    view->setRender(win);
//    XEGL::getInstance()->init(win);
//    XShader xShader;
//    xShader.init();

}