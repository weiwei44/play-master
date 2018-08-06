//
// Created by BMW on 2018/8/1.\
// 观察者
//

#ifndef PLAYMASTER_IOBSERVER_H
#define PLAYMASTER_IOBSERVER_H

#include "XData.h"
#include <vector>
#include <mutex>
#include "XThread.h"

class IObserver : public XThread{
public:
    //观察者接受数据的函数
    virtual void update(XData data){}
    //添加观察者
    void addObserver(IObserver* obs);

    //通知所有观察者
    void notify(XData data);

protected:
    std::vector<IObserver *> obss;
    std::mutex mux;  //线程安全锁
};


#endif //PLAYMASTER_IOBSERVER_H
