#include "previewwidget.h"
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

#define PREVIEW_WINDOW_WIDTH 200
#define PREVIEW_WINDOW_HEIGHT 100

PreviewWidget::PreviewWidget(QWidget *parent)
    : QWidget{parent, Qt::FramelessWindowHint}
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setFixedSize(PREVIEW_WINDOW_WIDTH, PREVIEW_WINDOW_HEIGHT);
    // _img = new QImage(200, 100, QImage::Format_RGB32);
    // for (int i = 0; i < 200; ++i)
    //     for (int j = 0; j < 100; ++j)
    //         _img->setPixel(i, j, 0xff0000);
    QImage img(PREVIEW_WINDOW_WIDTH, PREVIEW_WINDOW_HEIGHT, QImage::Format_RGB32);
    _label = new QLabel(this);
    _label->setPixmap(QPixmap::fromImage(img));
}

PreviewWidget::~PreviewWidget()
{
    //delete _img;
    _label->clear();
}

void PreviewWidget::Start(QString fileName)
{
    _decoder = new PreviewDecoder(fileName, this);
    connect(_decoder, &PreviewDecoder::previewReady, this, [this](QImage image){
        _label->setPixmap(QPixmap::fromImage(image));
    });
}

void PreviewWidget::Stop()
{
    _decoder->close();
    _decoder->deleteLater();
    hide();
}

bool PreviewWidget::isRunning()
{
    if (!_decoder)  return false;
    return _decoder->isRunning();
}

void PreviewWidget::seek(int position)
{
    _decoder->seek(position);
}

PreviewDecoder::PreviewDecoder(const QString &filePath, QObject *parent) : QThread(parent), _filePath(filePath)
{
    init();
    _isRunnig = true;
    start();
}

PreviewDecoder::~PreviewDecoder()
{
    if (_packet)    av_packet_free(&_packet);
    if (_frame)     av_frame_free(&_frame);
    //if (_outFrame)     av_frame_free(&_outFrame);
    if (_decodeCtx) avcodec_free_context(&_decodeCtx);
    if (_fmtCtx)    avformat_close_input(&_fmtCtx);
    if (_swsCtx)    sws_freeContext(_swsCtx);
}

void PreviewDecoder::seek(int seek_location)
{
    _seek_index = seek_location;
    _cond.notify_one();
}

void PreviewDecoder::close()
{
    _isRunnig = false;
    _cond.notify_all();
}

bool PreviewDecoder::isRunning()
{
    return _isRunnig;
}

int PreviewDecoder::init()
{
    int ret = avformat_open_input(&_fmtCtx, _filePath.toUtf8().data(), nullptr, nullptr);
    RET(avformat_open_input);

    ret = avformat_find_stream_info(_fmtCtx, nullptr);
    RET(avformat_find_stream_info);
    //av_dump_format(_fmtCtx, 0, _filePath.toUtf8().data(), 0);
    //fflush(stderr);

    ret = initDecoder();
    RET(initDecoder);
    ret = initSws();
    RET(initSws);
    return 0;
}

void PreviewDecoder::run()
{
    while(_isRunnig) {
        std::unique_lock<std::mutex> lock(_mutex);
        //等待seek操作
        while (_seek_index == -1) {
            _cond.wait(lock);
            if (!_isRunnig) return;
        }
        int64_t ts = _seek_index / av_q2d(_fmtCtx->streams[videoStreamIndex]->time_base);
        if (av_seek_frame(_fmtCtx, videoStreamIndex, ts, AVSEEK_FLAG_BACKWARD) < 0) {
            qWarning() << "seek failed for position: " << _seek_index;
            continue;
        }

        avcodec_flush_buffers(_decodeCtx);
        bool frameDecoded = false;
        while (!frameDecoded) {
            int ret = av_read_frame(_fmtCtx, _packet);
            if (ret == AVERROR_EOF) {
                qDebug() << "The Preview are Ended";
                break;
            }
            if (_packet->stream_index == videoStreamIndex) {
                ret = avcodec_send_packet(_decodeCtx, _packet);
                av_packet_unref(_packet);
                CONTINUE(avcodec_send_packet);
                //循环等待更多输入数据
                while (ret >= 0) {
                    ret = avcodec_receive_frame(_decodeCtx, _frame);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
                    else    BREAK(avcodec_receive_frame);
                    frameDecoded = true;
                    QImage image(PREVIEW_WINDOW_WIDTH, PREVIEW_WINDOW_HEIGHT, QImage::Format_RGB888);
                    uint8_t* destData[1] = {image.bits()};
                    int destLinesize[1] = {static_cast<int>(image.bytesPerLine())};
                    sws_scale(_swsCtx, _frame->data, _frame->linesize, 0, _decodeCtx->height, destData, destLinesize);

                    emit previewReady(image);
                    break;
                }
            }
        }
        _seek_index = -1;
    }
}

int PreviewDecoder::initDecoder()
{
    int ret = av_find_best_stream(_fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    RET(av_find_best_stream);
    //qDebug() << ret;
    videoStreamIndex = ret;
    _stream = (_fmtCtx)->streams[ret];
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
}

int PreviewDecoder::initSws()
{
    _outSpec.width = PREVIEW_WINDOW_WIDTH;
    _outSpec.height = PREVIEW_WINDOW_HEIGHT;
    _outSpec.pixFmt = AV_PIX_FMT_RGB24;
    _outSpec.size = av_image_get_buffer_size(_outSpec.pixFmt, _outSpec.width, _outSpec.height, 1);
    _swsCtx = sws_getContext(_decodeCtx->width, _decodeCtx->height, _decodeCtx->pix_fmt,
                             _outSpec.width, _outSpec.height, _outSpec.pixFmt,
                             SWS_BILINEAR, nullptr, nullptr, nullptr);

    if (!_swsCtx) {
        qDebug() << "sws_getContext Error";
        return -1;
    }

    _frame = av_frame_alloc();
    if (!_frame) {
        qDebug() << "av_frame_alloc Error";
        return -1;
    }

    // _outFrame = av_frame_alloc();
    // if (!_outFrame) {
    //     qDebug() << "av_frame_alloc Error";
    //     return -1;
    // }

    _packet = av_packet_alloc();
    if (!_packet) {
        qDebug() << "av_packet_alloc Failed";
        return -1;
    }

    //分配内存给Frame的data数组
    // int ret = av_image_alloc(_outFrame->data, _outFrame->linesize, _outSpec.width, _outSpec.height, _outSpec.pixFmt, 1);
    // RET(av_image_alloc);
    return 0;
}
