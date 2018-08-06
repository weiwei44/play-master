//
// Created by BMW on 2018/8/2.
//

#ifndef PLAYMASTER_XTEXTURE_H
#define PLAYMASTER_XTEXTURE_H

enum XTextureType{
    XTEXTURE_YUV420P = 0,     //Y4 U1  V1
    XTEXTURE_NV12 = 25,       //Y4 UV1
    XTEXTURE_NV21 = 26        //Y4 VU1
};

class XTexture {
public:
    static XTexture* create();
    virtual bool init(void* win,XTextureType type = XTEXTURE_YUV420P) = 0;
    virtual void draw(unsigned char* data[],int width,int height) = 0;
};


#endif //PLAYMASTER_XTEXTURE_H
