#ifndef DECODER_H
#define DECODER_H

#include <QThread>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}
#include "blockqueue.h"

enum class State{
    STOP = 0,
    PLAYING,
    PAUSE
};

//音频解码器
class AudioDecoder : public QThread
{
    Q_OBJECT
public:
    AudioDecoder();
    ~AudioDecoder();
    int init(AVFormatContext** fmtCtx);
    int getIndex();     //返回流索引，-1为错误
    void pushPkt(AVPacket pkt);
    void free();
    void clearQueue();
    double getClock();

protected:
    void run() override;

public slots:
    void onStateChanged(State state);

signals:
    void clockChanged(double clock);

private:
    int initDecoder(AVFormatContext** fmtCtx);
    int initSwr();

private:
    //音频重采样参数结构体
    typedef struct {
        int sampleRate;
        AVSampleFormat sampleFmt;
        int chLayout;
        int chs;
        int bytesPerSampleFrame;
    }SwrSpec;

    AVCodecContext* _decodeCtx = nullptr;
    AVStream* _stream = nullptr;
    AVFrame* _inFrame = nullptr, *_outFrame = nullptr;
    SwrContext* _swrCtx = nullptr;
    SwrSpec _inSpec, _outSpec;

    QAudioFormat _fmt;
    QAudioOutput* _out = nullptr;
    QIODevice* _io = nullptr;

    double _clock = 0;
    BlockQueue<AVPacket> _pktList;

    State _state = State::PLAYING;
};

//视频解码器
class VideoDecoder : public QThread
{
    Q_OBJECT
public:
    //视频转换参数结构体
    typedef struct {
        int width;
        int height;
        AVPixelFormat pixFmt;
        int size;
    }SwsSpec;

    VideoDecoder();
    ~VideoDecoder();
    int init(AVFormatContext** fmtCtx);
    int getIndex();
    void pushPkt(AVPacket pkt);
    int getBufLength();
    void free();
    void clearQueue();
    double getClock();
    void setAudioExist(bool flag);

protected:
    void run() override;

public slots:
    void onStateChanged(State state);
    void onAudioClockChanged(double clock);

signals:
    void frameDecoded(uint8_t* data, SwsSpec spec);
    void clockChanged(double clock);

private:
    int initDecoder(AVFormatContext** fmtCtx);
    int initSws();

private:

    AVCodecContext* _decodeCtx = nullptr;
    AVStream* _stream = nullptr;
    AVFrame* _inFrame = nullptr;
    AVFrame* _outFrame = nullptr;
    SwsContext* _swsCtx = nullptr;
    SwsSpec _outSpec;

    double _clock = 0;
    double _audioClock = 0;
    bool _hasAudio = true;
    BlockQueue<AVPacket> _pktList;
    State _state = State::PLAYING;
};

#endif // DECODER_H
