#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "decoder.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/log.h>
}
#include <atomic>

//播放器类，完成所有音视频播放操作
class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

    void play();                        //播放操作
    void stop();                        //停止操作，使播放立即停止
    void pause();                       //暂停操作

    void setFileName(QString fileName); //设置输入文件名
    void seekTime(int time);        //进行跳转操作
    int getDurationTime();          //获取总时长
    bool hasAudio();
    bool hasVideo();
    State&& getState();             //获取播放器状态

signals:
    void stateChanged(State state); //状态改变信号
    void initFinished();            //播放器初始化信号
    void SyncTime(double value);    //同步信号，将音视频同步信号统一发送给窗口

private:
    void setState(State state);
    void fateError();               //发生致命错误时的处理，如编码器上下文打开失败
    void readFile();                //读取文件，在子线程运行
    void free();                    //释放操作

private:
    State _state = State::STOP;     //此变量有两线程访问的风险
    int _seekTime = -1;             //seek操作变量，值为seek的时间，单位s
    QString _fileName;
    VideoDecoder* _vDecoder = nullptr;
    bool _hasAudio = false;
    bool _hasVideo = false;

    AVFormatContext* _fmtCtx = nullptr;
    AVDictionary* _opt = nullptr;
};

#endif // PLAYER_H
