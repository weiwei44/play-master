//
// Created by BMW on 2018/8/2.
//

#include "XShader.h"
#include "AndroidLog.h"

#define GET_STR(x) #x
#define FLOAT_BYTE sizeof(float)

//顶点着色器glsl
static const char *vertexShader = GET_STR(
        attribute vec4 aPosition;
        attribute vec2 aTexCoord;
        varying vec2 vTexCoord;
        void main(){
            vTexCoord = vec2(aTexCoord.x,1.0-aTexCoord.y);
            gl_Position = aPosition;
        }
);

//片元着色器
static const char *fragYUV420P = GET_STR(
        precision mediump float;
        varying vec2 vTexCoord;
        uniform sampler2D yTexture;
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            gl_FragColor = vec4(rgb,1.0);
        }
);

//片元着色器,软解码和部分x86硬解码
static const char *fragNV12 = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(uvTexture,vTexCoord).a - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
        }
);

//片元着色器,软解码和部分x86硬解码
static const char *fragNV21 = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).a - 0.5;
            yuv.b = texture2D(uvTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
        }
);

bool XShader::init(XShaderType type) {
    vertexShaderId = initShader(vertexShader,GL_VERTEX_SHADER);
    switch (type)
    {
        case XSHADER_YUV420P:
            fragmentShaderId = initShader(fragYUV420P,GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV12:
            fragmentShaderId = initShader(fragNV12,GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV21:
            fragmentShaderId = initShader(fragNV21,GL_FRAGMENT_SHADER);
            break;
        default:
            LOGE("XSHADER format is error");
            return false;
    }

    programId = createProgram(vertexShaderId,fragmentShaderId);

    glUseProgram(programId);

    //加入顶点数据
    static float ver[] = {
            1.0f,-1.0f,0.0f,
            -1.0f,-1.0f,0.0f,
            1.0f,1.0f,0.0f,
            -1.0f,1.0f,0.0f,
    };
    GLuint aPosition = (GLuint)(glGetAttribLocation(programId, "aPosition"));
    glEnableVertexAttribArray(aPosition);
    glVertexAttribPointer(aPosition,3, //一个点有几个元素
                          GL_FLOAT, GL_FALSE ,3*FLOAT_BYTE , ver);

    //纹理坐标
    static float txt[] = {
            1.0f,0.0f ,
            0.0f,0.0f,
            1.0f,1.0f,
            0.0,1.0
    };
    GLuint aTexCoord = (GLuint)(glGetAttribLocation(programId, "aTexCoord"));
    glEnableVertexAttribArray(aTexCoord);
    glVertexAttribPointer(aTexCoord,2,GL_FLOAT, GL_FALSE ,2*FLOAT_BYTE , txt);

    //纹理初始化
    //设置纹理层
    glUniform1i(glGetUniformLocation(programId,"yTexture"),0); //对应纹理第1层
    switch (type) {
        case XSHADER_YUV420P:
            glUniform1i(glGetUniformLocation(programId,"uTexture"),1); //对应纹理第2层
            glUniform1i(glGetUniformLocation(programId,"vTexture"),2); //对应纹理第3层
            break;
        case XSHADER_NV21:
        case XSHADER_NV12:
            glUniform1i(glGetUniformLocation(programId, "uvTexture"), 1); //对于纹理第2层
            break;
    }


    return true;
}

void XShader::getTexture(unsigned int index, int width, int heght, unsigned char *buf,bool alpha) {
    unsigned int format = GL_LUMINANCE;
    if(alpha){
        format = GL_LUMINANCE_ALPHA;
    }
    if(textures[index] == 0){
        //纹理初始化
        glGenTextures(1,&textures[index]);
        //设置纹理属性
        glBindTexture(GL_TEXTURE_2D,textures[index]);
        //缩小放大的过滤器，线性插值
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        //设置纹理的格式和大小
        glTexImage2D(GL_TEXTURE_2D,
                     0,  // 0 默认
                     format, //gpu内部格式 亮度，灰度图
                     width,heght, //图像宽高，要为2的次方
                     0,              //边框
                     format, //数据的像素格式，要和上面一致
                     GL_UNSIGNED_BYTE, //像素的数据类型
                     NULL);          //纹理的数据

    }

    //激活第index层纹理,绑定到创建的纹理
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,textures[index]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,heght,format,GL_UNSIGNED_BYTE,buf);

}

void XShader::draw() {
    if(!programId){
        return;
    }
    //三维绘制
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}


/**
 * shader初始化
 */
GLuint  XShader::initShader(const char* code,GLuint type){
    GLuint shader = glCreateShader(type);
    if(shader == 0){
        LOGE("glCreateShader %d failed",type);
        return 0;
    }
    //加载shader
    glShaderSource(shader,
                   1,  //shader数量
                   &code, //shader代码
                   0    //代码长度
    );
    //编译shader
    glCompileShader(shader);
    //获取编译情况
    GLint status;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&status);
    if(status == 0){
        LOGE("glCompileShader failed");
        return 0;
    }
    return shader;
}

GLuint XShader::createProgram(GLuint vertexID, GLuint fragmentID) {
    GLuint program = glCreateProgram();
    if(program == 0){
        LOGE("glCreateProgram failed");
        return 0;
    }
    glAttachShader(program,vertexID);
    glAttachShader(program,fragmentID);
    //连接
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program,GL_LINK_STATUS,&status);
    if(status == GL_FALSE){
        LOGE("glLinkProgram failed");
        return 0;
    }
    return program;
}