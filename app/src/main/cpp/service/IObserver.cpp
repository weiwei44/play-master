//
// Created by BMW on 2018/8/1.
//

#include "IObserver.h"

void IObserver::addObserver(IObserver *obs) {
    if(!obs)
        return;
    mux.lock();

    obss.push_back(obs);

    mux.unlock();
}

void IObserver::notify(XData data) {
    mux.lock();

    for(int i = 0;i< obss.size();i++){
        obss[i]->update(data);
    }

    mux.unlock();
}