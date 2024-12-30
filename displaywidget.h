#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "decoder.h"

//OpenGL显示窗口，使用YUV420P数据，在shader中通过矩阵转为RGB
class DisplayWidget : public QOpenGLWidget, QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    DisplayWidget(QWidget* parent = nullptr);
    void free();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

public slots:
    void onFrameDecoded(uint8_t* data, VideoDecoder::SwsSpec spec);

private:
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLShaderProgram* glprogram = nullptr;
    QOpenGLTexture* textureYUV[3];
    uint8_t* _frame = nullptr;
    VideoDecoder::SwsSpec _spec;
};

#endif // DISPLAYWIDGET_H
