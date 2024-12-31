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
    static AudioDecoder& getInstall();

    int init(AVFormatContext** fmtCtx);
    int getIndex();     //返回流索引，-1为错误
    void pushPkt(AVPacket pkt);
    void free();
    void clearQueue();
    double getClock();
    void changeVolume(float volume);

protected:
    void run() override;

public slots:
    void onStateChanged(State state);

signals:
    void clockChanged(double clock);

private:
    //禁止外部构造与析构
    AudioDecoder();
    ~AudioDecoder();
    int initDecoder(AVFormatContext** fmtCtx);
    int initSwr();

    int16_t clampInt16(int32_t value);          //阈值限定
    void reducePcmVolum(int16_t *pcm_data, size_t num_samples);     //降低PCM数据的音量

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
    float _volume = 1.0;

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

    static VideoDecoder& getInstall();
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
    //禁止外部构造与析构
    VideoDecoder();
    ~VideoDecoder();
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
