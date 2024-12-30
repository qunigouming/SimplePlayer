#include "decoder.h"
#include <QDebug>
#include <QTime>
extern "C" {
#include <libavutil/imgutils.h>
}

#define DEFAULT_VIDEO_FRAME 25
static double videoFrame = DEFAULT_VIDEO_FRAME;

#define ERROR_BUF \
char errbuf[1024]; \
    av_strerror(ret, errbuf, sizeof(errbuf));

#define CODE(func, code) \
if (ret < 0) { \
        ERROR_BUF; \
        qDebug() << __LINE__ << #func << "error" << errbuf; \
        code; \
}

#define END(func) CODE(func, fateError(); return;)
#define RET(func) CODE(func, return ret;)
#define CONTINUE(func) CODE(func, continue;)
#define BREAK(func) CODE(func, break;)

VideoDecoder::VideoDecoder() {

}

VideoDecoder::~VideoDecoder()
{
    free();
}

int VideoDecoder::init(AVFormatContext **fmtCtx)
{
    int ret = initDecoder(fmtCtx);
    RET(initDecoder);
    ret = initSws();
    RET(initSws);
    return 0;
}

int VideoDecoder::getIndex()
{
    if (!_stream) {
        qDebug() << "stream is empty";
        return -1;
    }
    return _stream->index;
}

void VideoDecoder::pushPkt(AVPacket pkt)
{
    _pktList.push_back(pkt);
}

int VideoDecoder::getBufLength()
{
    return _pktList.size();
}

void VideoDecoder::free()
{
    _pktList.clear();
    av_frame_free(&_inFrame);
    if (_outFrame) {
        av_freep(&_outFrame->data[0]);
        av_frame_free(&_outFrame);
    }
    sws_freeContext(_swsCtx);
    _swsCtx = nullptr;
    _stream = nullptr;
    avcodec_free_context(&_decodeCtx);
    //_state = State::PLAYING;
    qDebug() << "释放视频相关资源";
}

void VideoDecoder::clearQueue()
{
    _clock = 0;
    _pktList.clear();
}

double VideoDecoder::getClock()
{
    return _clock;
}

void VideoDecoder::setAudioExist(bool flag)
{
    _hasAudio = flag;
}

void VideoDecoder::run()
{
    int frameCount = 1000 / videoFrame;
    QTime t1;
    t1.start();
    while (true) {
        if (_state == State::STOP) {
            qDebug() << "收到停止信号";
            break;
        }
        if (_pktList.empty() || _state == State::PAUSE)   continue;
        AVPacket pkt;
        _pktList.pop(pkt);

        if (pkt.pts != AV_NOPTS_VALUE) {
            _clock = av_q2d(_stream->time_base) * pkt.pts;
            //无音频时，采用视频同步
            if (!_hasAudio)
                emit clockChanged(_clock);
        }
        int ret = avcodec_send_packet(_decodeCtx, &pkt);
        av_packet_unref(&pkt);
        CONTINUE(avcodec_send_packet);

        while (true) {
            ret = avcodec_receive_frame(_decodeCtx, _inFrame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)   break;
            else    BREAK(avcodec_receive_frame);
            sws_scale(_swsCtx, _inFrame->data, _inFrame->linesize, 0, _decodeCtx->height, _outFrame->data, _outFrame->linesize);

            while (_hasAudio && _clock >= _audioClock && _state == State::PLAYING);
            //将数据拷贝出来，防止两个线程访问资源
            uint8_t* data = (uint8_t*)av_malloc(_outSpec.size);
            memcpy(data, _outFrame->data[0], _outSpec.size);
            if (!_hasAudio) {
                while (t1.elapsed() < frameCount);      //控制图像到指定时间刷新
                t1.restart();
            }
            // AVFrame* frame = av_frame_alloc();
            // frame->format = _outFrame->format;
            // frame->width = _outFrame->width;
            // frame->height = _outFrame->height;
            // ret = av_image_alloc(frame->data, frame->linesize, _outSpec.width, _outSpec.height, _outSpec.pixFmt, 1);
            // av_frame_copy(frame, _outFrame);
            emit frameDecoded(data, _outSpec);
        }
    }
    free();
    qDebug() << "视频线程退出";
}

void VideoDecoder::onStateChanged(State state)
{
    _state = state;
}

void VideoDecoder::onAudioClockChanged(double clock)
{
    _audioClock = clock;
}

int VideoDecoder::initDecoder(AVFormatContext **fmtCtx)
{
    int ret = av_find_best_stream(*fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    RET(av_find_best_stream);
    //qDebug() << ret;
    _stream = (*fmtCtx)->streams[ret];
    if (!_stream) {
        qDebug() << "Video stream is empty!";
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
    //根据视频读出来的fps来设置播放的fps
    AVRational fps = _stream->r_frame_rate;
    videoFrame = fps.num / fps.den;
    //qDebug() << _stream->r_frame_rate.num << "/" << _stream->r_frame_rate.den;
}

int VideoDecoder::initSws()
{
    // int w = 800;
    // int h = 600;
    // int dx = 0;
    // int dy = 0;
    // int dw = _spec.width;
    // int dh = _spec.height;
    // if (dw > w || dh > h) {
    //     if (dw * h > w * dh) {
    //         dh = w * dh / dw;
    //         dw = w;
    //     }
    //     else {
    //         dw = h * dw / dh;
    //         dh = h;
    //     }
    // }
    // dx = (w - dw) >> 1;
    // dy = (h - dh) >> 1;
    _outSpec.width = 800;
    _outSpec.height = 600;
    _outSpec.pixFmt = AV_PIX_FMT_YUV420P;
    _outSpec.size = av_image_get_buffer_size(_outSpec.pixFmt, _outSpec.width, _outSpec.height, 1);
    _swsCtx = sws_getContext(_decodeCtx->width, _decodeCtx->height, _decodeCtx->pix_fmt,
                              _outSpec.width, _outSpec.height, _outSpec.pixFmt,
                              SWS_BILINEAR, nullptr, nullptr, nullptr);

    if (!_swsCtx) {
        qDebug() << "sws_getContext Error";
        return -1;
    }

    _inFrame = av_frame_alloc();
    if (!_inFrame) {
        qDebug() << "av_frame_alloc Error";
        return -1;
    }
    _outFrame = av_frame_alloc();
    if (!_outFrame) {
        qDebug() << "av_frame_alloc Error";
        return -1;
    }

    //分配内存给Frame的data数组
    int ret = av_image_alloc(_outFrame->data, _outFrame->linesize, _outSpec.width, _outSpec.height, _outSpec.pixFmt, 1);
    RET(av_image_alloc);
    return 0;
}
