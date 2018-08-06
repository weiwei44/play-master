//
// Created by BMW on 2018/8/2.
//

#include <android/native_window.h>
#include <EGL/egl.h>

#include "XEGL.h"
#include "AndroidLog.h"

class CXEGL : public XEGL{
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    virtual void draw(){
        if(display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE){
            return;
        }
        eglSwapBuffers(display,surface);
    }
    virtual bool init(void* win){
        ANativeWindow *nativeWindow = (ANativeWindow*)win;

        //display init
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if(display == EGL_NO_DISPLAY){
            LOGE("eglGetDisplay failed");
            return false;
        }
        if(EGL_FALSE == eglInitialize(display,0,0)){
            LOGE("eglInitialize failed");
            return false;
        }

        //config surface
        EGLint configList[] = {
                EGL_RED_SIZE,8,
                EGL_GREEN_SIZE,8,
                EGL_BLUE_SIZE,8,
                EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config = 0;
        EGLint numConfig = 0;
        if(EGL_TRUE != eglChooseConfig(display,configList,&config,1,&numConfig)){
            LOGE("eglChooseConfig failed");
            return false;
        }
        surface = eglCreateWindowSurface(display,config,nativeWindow,NULL);

        //create context
        const EGLint contextList[] = {
                EGL_CONTEXT_CLIENT_VERSION,2,
                EGL_NONE
        };
        context = eglCreateContext(display,config,EGL_NO_CONTEXT,contextList);
        if(EGL_NO_CONTEXT == context){
            LOGE("eglCreateContext failed");
            return false;
        }
        if(EGL_TRUE != eglMakeCurrent(display,surface,surface,context)){
            LOGE("eglMakeCurrent failed");
            return false;
        }

        LOGE("egl success");
        return true;
    }
};


XEGL* XEGL::getInstance(){
    static CXEGL egl;
    return &egl;
}
