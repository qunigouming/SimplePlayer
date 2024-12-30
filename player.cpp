#include "player.h"
#include <thread>
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
#define CONTINUE(func) CODE(func, continue)
#define BREAK(func) CODE(func, break)

Player::Player(QObject *parent)
    : QObject{parent}
{
    //av_log_set_level(AV_LOG_DEBUG);
    _aDecoder = new AudioDecoder();
    connect(this, &Player::stateChanged, _aDecoder, &AudioDecoder::onStateChanged);
    _vDecoder = new VideoDecoder();
    connect(this, &Player::stateChanged, _vDecoder, &VideoDecoder::onStateChanged);
    connect(_aDecoder, &AudioDecoder::clockChanged, this, [this](double value){
        _vDecoder->onAudioClockChanged(value);
        emit SyncTime(value);
    });
    connect(_vDecoder, &VideoDecoder::clockChanged, this, [this](double value){
        emit SyncTime(value);
    });
    connect(_vDecoder, &VideoDecoder::finished, this, [this]{
        qDebug() << "结束视频线程";
    });
    avformat_network_init();
    av_dict_set(&_opt, "rtsp_transport", "tcp", 0);
    av_dict_set(&_opt, "max_delay", "550", 0);
}

void Player::play()
{
    if (_state == State::PAUSE) {
        setState(State::PLAYING);
        return;
    }
    setState(State::PLAYING);
    std::thread([this]{
        try {
            readFile();
        }
        catch (...) {
            qDebug() << "捕获到异常";
        }
    }).detach();
}

void Player::stop()
{
    setState(State::STOP);          //设置为STOP状态，音频解码线程和视频解码线程都会进入退出状态，读取文件线程也进入退出态，此时会调用free()释放资源,避免多线程访问资源
    //free();
}

void Player::pause()
{
    if (_state != State::PLAYING)  return;
    setState(State::PAUSE);
}

void Player::setFileName(QString fileName)
{
    _fileName = fileName;
}

void Player::seekTime(int time)
{
    _seekTime = time;
}

int Player::getDurationTime()
{
    return _fmtCtx ? round(_fmtCtx->duration * av_q2d(AV_TIME_BASE_Q)) : 0;
}

State&& Player::getState()
{
    return std::move(_state);
}

VideoDecoder *Player::getVideoPtr()
{
    return _vDecoder;
}

AudioDecoder *Player::getAudioPtr()
{
    return _aDecoder;
}

void Player::setState(State state)
{
    if (state == _state) return;
    _state = state;
    emit stateChanged(_state);
}

void Player::fateError()
{
    qDebug() << "遇到致命错误";
    setState(State::STOP);
    free();
}

void Player::readFile()
{
    qDebug() << _fileName.toUtf8().data();
    //_fileName = "https://media.w3.org/2010/05/sintel/trailer.mp4";
    //_fileName = "http://devimages.apple.com/iphone/samples/bipbop/gear1/prog_index.m3u8";
    //_fileName = "https://liveop.cctv.cn/hls/4KHD/playlist.m3u8";
    //_fileName = "https://vs-cmaf-push-ww-live.akamaized.net/x=4/i=urn:bbc:pips:service:bbc_news_channel_hd/iptv_hd_abr_v1.mpd";
    int ret = avformat_open_input(&_fmtCtx, _fileName.toUtf8().data(), nullptr, &_opt);
    END(avformat_open_input);

    ret = avformat_find_stream_info(_fmtCtx, nullptr);
    END(avformat_find_stream_info);
    av_dump_format(_fmtCtx, 0, _fileName.toUtf8().data(), 0);
    fflush(stderr);

    if (_aDecoder->init(&_fmtCtx) >= 0) {
        _hasAudio = true;
        _aDecoder->start();
    }

    if (_vDecoder->init(&_fmtCtx) >= 0) {
        _hasVideo = true;
        _vDecoder->setAudioExist(_hasAudio);
        _vDecoder->start();
    }
    //seek操作使用的索引
    int streamIndex = _hasAudio ? _aDecoder->getIndex() : _hasVideo ? _vDecoder->getIndex() : 0;
    emit initFinished();
    AVPacket pkt;
    while (_state != State::STOP) {
        //qDebug() << _state;
        //进行seek操作
        //qDebug() << "视频缓存队列：" << _vDecoder->getBufLength();
        if (_seekTime >= 0) {
            int64_t ts = _seekTime / av_q2d(_fmtCtx->streams[streamIndex]->time_base);
            ret = av_seek_frame(_fmtCtx, streamIndex, ts, AVSEEK_FLAG_BACKWARD);
            if (ret >= 0) {
                if (_hasAudio)
                    _aDecoder->clearQueue();
                if (_hasVideo)
                    _vDecoder->clearQueue();
            }
            _seekTime = -1;
        }
        ret = av_read_frame(_fmtCtx, &pkt);
        //给音视频缓冲区发送数据，使各自线程进行解码操作
        if (ret == 0) {
            if (_hasAudio && pkt.stream_index == _aDecoder->getIndex()) {
                _aDecoder->pushPkt(pkt);
            }
            else if (_hasVideo && pkt.stream_index == _vDecoder->getIndex()) {
                _vDecoder->pushPkt(pkt);
            }
            else av_packet_unref(&pkt);
        }
        //读取到末尾
        else if (ret == AVERROR_EOF) {
            //qDebug() << "read file finished!!!";
            //break;
        }
        //错误处理
        else {
            ERROR_BUF;
            qDebug() << "av_read_frame Error" << errbuf;
            continue;
        }
    }
    qDebug() << "退出读取";
    free();
}

void Player::free()
{
    qDebug() << "开始释放资源";
    //等待线程退出再释放AVFormatContext
    if (_hasAudio) {
        _aDecoder->quit();
        _aDecoder->wait();
        // _aDecoder->free();           //将释放交给各自解码器管理
    }
    if (_hasVideo) {
        _vDecoder->quit();
        _vDecoder->wait();
        // _vDecoder->free();
    }
    _hasAudio = false;
    _hasVideo = false;
    if (_fmtCtx) {
        avformat_close_input(&_fmtCtx);
        _fmtCtx = nullptr;
    }
    qDebug() << "释放资源完毕";
}
