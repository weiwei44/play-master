//
// Created by BMW on 2018/8/2.
//

#ifndef PLAYMASTER_XEGL_H
#define PLAYMASTER_XEGL_H


class XEGL {
public:
    virtual bool init(void* win) = 0;
    virtual void draw() = 0;
    static XEGL* getInstance();

protected:
    XEGL(){}

};


#endif //PLAYMASTER_XEGL_H
