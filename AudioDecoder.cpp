#include "decoder.h"
#include <QDebug>

#define ERROR_BUF \
char errbuf[1024]; \
    av_strerror(ret, errbuf, sizeof(errbuf));

#define CODE(func, code) \
if (ret < 0) { \
        ERROR_BUF; \
        qDebug() << #func << "error" << errbuf; \
        code; \
}

#define END(func) CODE(func, fateError(); return;)
#define RET(func) CODE(func, return ret;)
#define CONTINUE(func) CODE(func, continue;)
#define BREAK(func) CODE(func, break;)

AudioDecoder::AudioDecoder() {
}

AudioDecoder::~AudioDecoder()
{
    free();
}

int AudioDecoder::init(AVFormatContext **fmtCtx)
{
    int ret = initDecoder(fmtCtx);
    RET(initDecoder);
    ret = initSwr();
    RET(initSwr);
    _fmt.setSampleRate(44100);       //采样率
    _fmt.setSampleSize(16);
    _fmt.setChannelCount(2);         //通道数
    _fmt.setCodec("audio/pcm");      //编码器格式
    _fmt.setByteOrder(QAudioFormat::LittleEndian);
    _fmt.setSampleType(QAudioFormat::UnSignedInt);
    _out = new QAudioOutput(_fmt);
    _io = _out->start();
    return 0;
}

int AudioDecoder::getIndex()
{
    if (!_stream) {
        qDebug() << "audio stream is empty";
        return -1;
    }
    return _stream->index;
}

void AudioDecoder::pushPkt(AVPacket pkt)
{
    _pktList.push_back(pkt);
}

void AudioDecoder::free()
{
    qDebug() << "free source";
    _stream = nullptr;
    _pktList.clear();
    av_frame_free(&_inFrame);
    if (_outFrame) {
        av_freep(&_outFrame->data[0]);
        av_frame_free(&_outFrame);
    }
    swr_free(&_swrCtx);
    avcodec_free_context(&_decodeCtx);
    _io->close();
    _out->deleteLater();
    //_state = State::PLAYING;
}

void AudioDecoder::clearQueue()
{
    _clock = 0;
    _pktList.clear();
}

double AudioDecoder::getClock()
{
    return _clock;
}

void AudioDecoder::run()
{
    while (true) {
        if (_state == State::STOP) {
            break;
        }
        if (_pktList.empty() || _state == State::PAUSE) continue;
        AVPacket pkt;
        _pktList.pop(pkt);

        //为可用时间戳
        if (pkt.pts != AV_NOPTS_VALUE) {
            _clock = av_q2d(_stream->time_base) * pkt.pts;
            emit clockChanged(_clock);
        }

        int ret = avcodec_send_packet(_decodeCtx, &pkt);
        av_packet_unref(&pkt);
        CONTINUE(avcodec_send_packet);
        //while (true) {        //解码器在收到第一个Packet时因为需要更多数据，会返回-11错误以期望更多数据输入
            ret = avcodec_receive_frame(_decodeCtx, _inFrame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)   CONTINUE(avcodec_receive_frame);
            int outSamples = av_rescale_rnd(_outSpec.sampleRate, _inFrame->nb_samples, _inSpec.sampleRate, AV_ROUND_UP);
            ret = swr_convert(_swrCtx, _outFrame->data, outSamples, (const uint8_t**)_inFrame->data, _inFrame->nb_samples);
            BREAK(swr_convert);
        //}
        //异常数据
        if (ret < 0) {
            qDebug() << "abnormal data";
        }
        int size = ret * _outSpec.bytesPerSampleFrame;
        while (_out->bytesFree() < size);       //等待足够的字节缓存
        _io->write((const char*)_outFrame->data[0], size);
    }
    free();
}

void AudioDecoder::onStateChanged(State state)
{
    //qDebug() << "Current State:" << (int)state;
    _state = state;
}

int AudioDecoder::initDecoder(AVFormatContext **fmtCtx)
{
    int ret = av_find_best_stream(*fmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    RET(av_find_best_stream);
    _stream = (*fmtCtx)->streams[ret];
    if (!_stream) {
        qDebug() << "Audio stream is empty!";
        return -1;
    }

    AVCodec* decoder = avcodec_find_decoder(_stream->codecpar->codec_id);
    if (!decoder) {
        qDebug() << "decoder not found" << _stream->codecpar->codec_id;
        return -1;
    }
    _decodeCtx = avcodec_alloc_context3(decoder);
    if (!_decodeCtx) {
        qDebug() << "avcodec_alloc_context3 error";
        return -1;
    }
    ret = avcodec_parameters_to_context(_decodeCtx, _stream->codecpar);
    RET(avcodec_parameters_to_context);
    ret = avcodec_open2(_decodeCtx, decoder, nullptr);
    RET(avcodec_open2);
}

int AudioDecoder::initSwr()
{
    _inSpec.sampleRate = _decodeCtx->sample_rate;
    _inSpec.sampleFmt = _decodeCtx->sample_fmt;
    _inSpec.chs = _decodeCtx->channels;
    _inSpec.chLayout = _decodeCtx->channel_layout;

    _outSpec.sampleRate = 44100;
    _outSpec.sampleFmt = AV_SAMPLE_FMT_S16;
    _outSpec.chLayout = AV_CH_LAYOUT_STEREO;
    _outSpec.chs = av_get_channel_layout_nb_channels(_outSpec.chLayout);
    _outSpec.bytesPerSampleFrame = _outSpec.chs * av_get_bytes_per_sample(_outSpec.sampleFmt);
    _swrCtx =   swr_alloc_set_opts(nullptr, _outSpec.chLayout, _outSpec.sampleFmt, _outSpec.sampleRate,
                                _inSpec.chLayout, _inSpec.sampleFmt, _inSpec.sampleRate, 0, nullptr);
    if (!_swrCtx) {
        qDebug() << "swr_alloc_set_opts Error";
        return -1;
    }
    int ret = swr_init(_swrCtx);
    RET(swr_init);
    _inFrame = av_frame_alloc();
    if (!_inFrame) {
        qDebug() << "in avframe alloc Error";
        return -1;
    }
    _outFrame = av_frame_alloc();
    if (!_outFrame) {
        qDebug() << "out avframe alloc Error";
        return -1;
    }
    ret = av_samples_alloc(_outFrame->data, _outFrame->linesize, _outSpec.chs, 4096, _outSpec.sampleFmt, 1);
    RET(av_samples_alloc);
    return 0;
}
