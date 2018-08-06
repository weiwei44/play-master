//
// Created by BMW on 2018/8/2.
//

#include "XTexture.h"
#include "AndroidLog.h"
#include "XEGL.h"
#include "XShader.h"

class CXTexture: public XTexture{
public:
    XShader xShader;
    XTextureType type;
    virtual bool init(void* win,XTextureType type){
        this->type = type;
        if(!win){
            return false;
        }
        if(!XEGL::getInstance()->init(win)){
            return false;
        }
        xShader.init((XShaderType)(type));
        return true;
    }
    virtual void draw(unsigned char* data[],int width,int height){
        xShader.getTexture(0,width,height,data[0]);  //Y

        if(type == XTEXTURE_YUV420P){
            xShader.getTexture(1,width/2,height/2,data[1]); //U
            xShader.getTexture(2,width/2,height/2,data[2]);  //V
        }else {
            xShader.getTexture(1, width / 2, height / 2, data[1], true); //uv
        }
        xShader.draw();
        XEGL::getInstance()->draw();
    }
};

XTexture* XTexture::create() {
    return new CXTexture();
}