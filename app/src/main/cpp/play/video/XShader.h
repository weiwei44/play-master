//
// Created by BMW on 2018/8/2.
//

#ifndef PLAYMASTER_XSHADER_H
#define PLAYMASTER_XSHADER_H



#include <GLES2/gl2.h>

enum XShaderType{
    XSHADER_YUV420P = 0,     //Y4 U1  V1
    XSHADER_NV12 = 25,       //Y4 UV1
    XSHADER_NV21 = 26        //Y4 VU1
};

class XShader {
public:
    virtual bool init(XShaderType type = XSHADER_YUV420P );
    /**
     * 获取纹理，并映射到内存
     * @param index  纹理索引
     * @param alpha  是否有透明通道
     */
    virtual void getTexture(unsigned int index,int width,int heght, unsigned char *buf, bool alpha = false);
    virtual void draw();

protected:
    GLuint  initShader(const char* code,GLuint type);
    GLuint createProgram(GLuint vertexID,GLuint fragmentID);

    GLuint programId = 0;
    GLuint vertexShaderId = 0;
    GLuint fragmentShaderId = 0;
    unsigned int textures[100] = {0};  //缓存纹理

};


#endif //PLAYMASTER_XSHADER_H
