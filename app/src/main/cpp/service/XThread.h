//
// Created by BMW on 2018/8/1.
//

#ifndef PLAYMASTER_XTHREAD_H
#define PLAYMASTER_XTHREAD_H

/**
 * 线程停止
 * @param ms 毫秒
 */
void xSleep(int ms);

class XThread {
public:
    virtual void start();
    virtual void stop();
    virtual void service(){}

protected:
    bool isExit = false;  //线程是否退出
    bool isRunning = false;  //线程是否运行中
private:
    void threadService();
};


#endif //PLAYMASTER_XTHREAD_H
