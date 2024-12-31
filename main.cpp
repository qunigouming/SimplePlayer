/*
 *缺陷：视频内含有B帧时，进度条会一动一动的，需解决视频解码器_clock采用时间问题
 *
 *v1.1添加录屏功能
*/
#include "mainwidget.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<State>("State");
    qRegisterMetaType<VideoDecoder::SwsSpec>("SwsSpec");
    //导入字体库
    QFontDatabase::addApplicationFont(":/font/source/font/volume.ttf");
    MainWidget w;
    w.show();
    return a.exec();
}
