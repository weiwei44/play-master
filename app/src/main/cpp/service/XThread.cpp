//
// Created by BMW on 2018/8/1.
//

#include "XThread.h"
#include "AndroidLog.h"
#include <thread>
using namespace std;

void xSleep(int ms){
    chrono::milliseconds du(ms);  //定义一个毫秒时间
    this_thread::sleep_for(du);
}

void XThread::start() {
    isExit = false;
    thread t(&XThread::threadService,this);  //这里线程已经启动
    t.detach();  //当前放弃对新创建线程的控制

}

//通过控制isExit安全停止线程（不一定成功）
void XThread::stop() {
    isExit = true;
    for(int i = 0;i < 200; i++){  //给线程200ms毫秒来停止
        if(!isRunning){
            LOGD("stop serveice success");
            return;
        }
        xSleep(1);
    }
    LOGE("stop service timeout");
}

void XThread::threadService() {
    isRunning = true;
    service();
    isRunning = false;
}