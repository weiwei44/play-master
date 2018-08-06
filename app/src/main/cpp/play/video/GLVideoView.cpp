//
// Created by BMW on 2018/8/2.
//

#include "GLVideoView.h"
#include "AndroidLog.h"
#include "XTexture.h"

void GLVideoView::render(XData xData) {

    if(!view){
        return;
    }
    if(!texture){
        texture = XTexture::create();
        texture->init(view, (XTextureType)(xData.format));
    }
    texture->draw(xData.datas,xData.width,xData.height);
}

void GLVideoView::setRender(void *win) {
    view = win;
}