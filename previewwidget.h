#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QThread>
#include <mutex>
#include <QLabel>
#include <memory>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

class PreviewDecoder : public QThread
{
    Q_OBJECT
public:
    typedef struct {
        int width;
        int height;
        AVPixelFormat pixFmt;
        int size;
    }SwsSpec;

    PreviewDecoder(const QString& filePath, QObject* parent = nullptr);
    ~PreviewDecoder();
    void seek(int seek_location);
    void close();
    bool isRunning();

    int init();

signals:
    void previewReady(QImage image);

protected:
    void run() override;

private:
    int initDecoder();
    int initSws();

    QString _filePath;
    bool _isRunnig = false;
    AVFormatContext* _fmtCtx = nullptr;
    AVCodecContext* _decodeCtx = nullptr;
    AVStream* _stream = nullptr;
    AVFrame* _frame = nullptr;
    // AVFrame* _inFrame = nullptr;
    // AVFrame* _outFrame = nullptr;
    AVPacket* _packet = nullptr;
    SwsContext* _swsCtx = nullptr;
    SwsSpec _outSpec;

    std::mutex _mutex;
    std::condition_variable _cond;
    int videoStreamIndex = -1;
    int _seek_index = -1;
};

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);
    ~PreviewWidget();
    void Start(QString fileName);
    void Stop();
    bool isRunning();
    void seek(int position);

private:
    //QImage* _img;
    QLabel* _label;
    PreviewDecoder* _decoder = nullptr;
};

#endif // PREVIEWWIDGET_H
