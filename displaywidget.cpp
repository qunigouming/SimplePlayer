#include "displaywidget.h"
#include <QDebug>

DisplayWidget::DisplayWidget(QWidget *parent)
{

}

void DisplayWidget::free()
{
    if (_frame) {
        av_free(_frame);
        _frame = nullptr;
    }
    update();
}

void DisplayWidget::initializeGL()
{
    initializeOpenGLFunctions();

    static float vertices[] = {
        //顶点坐标               //纹理坐标的Y方向需要是反的,因为opengl中的坐标系是Y原点位于下方
        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f,        //左下
        1.0f , -1.0f, 0.0f,  1.0f, 1.0f,        //右下
        -1.0f, 1.0f,  0.0f,  0.0f, 0.0f,        //左上
        1.0f,  1.0f,  0.0f,  1.0f, 0.0f         //右上
    };

    glprogram = new QOpenGLShaderProgram(this);
    glprogram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shapes/shapes.vert");
    glprogram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shapes/shapes.frag");
    bool ret = glprogram->link();
    if (!ret) av_log(nullptr, AV_LOG_ERROR, "The program have error:%s\n", glprogram->log());
    glprogram->bind();

    //初始化纹理对象
    for(int i=0;i<3;i++)
    {
        textureYUV[i]  = new QOpenGLTexture(QOpenGLTexture::Target2D);

        if(i==0)
        {
            textureYUV[i]->setSize(this->width(),this->height());
        }
        else
        {
            textureYUV[i]->setSize(this->width()/2,this->height()/2);
        }

        textureYUV[i]->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        textureYUV[i]->create();
        textureYUV[i]->setFormat(QOpenGLTexture::R8_UNorm);
        textureYUV[i]->allocateStorage();        //存储配置(放大缩小过滤、格式、size)
        //textureYUV[i]->setData(QOpenGLTexture::Red,QOpenGLTexture::UInt8, m_frame->data[i]);
    }
    vbo.create();
    vbo.bind();              //绑定到当前的OpenGL上下文,
    vbo.allocate(vertices, sizeof(vertices));
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);  //设置为一次修改，多次使用(坐标不变,变得只是像素点)

    //初始化VAO,设置顶点数据状态(顶点，法线，纹理坐标等)
    vao.create();
    vao.bind();

    // void setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0);
    glprogram->setAttributeBuffer(0, GL_FLOAT, 0,                  3, 5 * sizeof(float));   //设置aPos顶点属性
    glprogram->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float),  2, 5 * sizeof(float));   //设置aColor顶点颜色

    glprogram->enableAttributeArray(0); //使能
    glprogram->enableAttributeArray(1);

    //解绑所有对象
    vao.release();
    vbo.release();
}

void DisplayWidget::resizeGL(int w, int h)
{

}

void DisplayWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (_frame) {
        vao.bind();
        glprogram->setUniformValue("tex_y", 0);
        glprogram->setUniformValue("tex_u", 1);
        glprogram->setUniformValue("tex_v", 2);

        // for(int i=0;i<3;i++)
        // {
        //     textureYUV[i]->setData(QOpenGLTexture::Red,QOpenGLTexture::UInt8, _frame);
        //     textureYUV[i]->bind(i);
        // }
        textureYUV[0]->setData(QOpenGLTexture::Red,QOpenGLTexture::UInt8, _frame);
        textureYUV[0]->bind(0);
        textureYUV[1]->setData(QOpenGLTexture::Red,QOpenGLTexture::UInt8, _frame + _spec.width * _spec.height);
        textureYUV[1]->bind(1);
        textureYUV[2]->setData(QOpenGLTexture::Red,QOpenGLTexture::UInt8, _frame + (_spec.width * _spec.height)*5/4);
        textureYUV[2]->bind(2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        textureYUV[0]->release();
        textureYUV[1]->release();
        textureYUV[2]->release();

        vao.release();       //解绑

        av_free(_frame);
        _frame = nullptr;
    }
}

void DisplayWidget::onFrameDecoded(uint8_t* data, VideoDecoder::SwsSpec spec)
{
    // if (_frame) {
    //     av_free(_frame);
    //     _frame = nullptr;
    // }
    _frame = data;
    _spec = spec;
    update();
}


