//
// Created by BMW on 2018/8/2.
//

#ifndef PLAYMASTER_GLVIDEOVIEW_H
#define PLAYMASTER_GLVIDEOVIEW_H

#include "IVideoView.h"

class XTexture;

class GLVideoView : public IVideoView {
public:
    virtual void setRender(void* win);
    virtual void render(XData xData);

protected:
    void* view = 0;
    XTexture* texture = 0;
};


#endif //PLAYMASTER_GLVIDEOVIEW_H
