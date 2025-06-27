#include "mainwidget.h"
#include "./ui_mainwidget.h"
#include <QFileDialog>
#include "displaywidget.h"
#include <QEvent>
#include <QKeyEvent>
#include <QScreen>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    _player = new Player(this);
    ui->fileCombox->addItem(QString());
    ui->fileCombox->addItem("https://vs-cmaf-push-ww-live.akamaized.net/x=4/i=urn:bbc:pips:service:bbc_news_channel_hd/iptv_hd_abr_v1.mpd");
    ui->fileCombox->addItem("rtmp://192.168.186.130:1935/hls1");
    ui->fileCombox->addItem("http://192.168.186.130/audio/Thriller.mp3");
    ui->fileCombox->setCurrentIndex(0);
    _volumeWidget = new Volume(this);
    _volumeWidget->installEventFilter(this);
    ui->volumeBtn->installEventFilter(this);
    ui->currentSlider->installEventFilter(this);
    _volumeWidget->hide();
    ui->volumeBtn->setFont(QFont("volume"));
    ui->volumeBtn->setText(QChar(0xea0d));

    _preview = new PreviewWidget(this);
    _preview->hide();
    //ui->currentSlider->setMouseTracking(true);
    connect(_player, &Player::stateChanged, this, &MainWidget::onStateChanged);
    connect(_player, &Player::initFinished, this, &MainWidget::onInitFinished);
    //connect第五个参数不填默认为AutoConnection，既两者同线程下用DirectConnection，异线程用QueuedConnection，使用事件队列处理信号槽，可能会导致OpenGL访问空指针问题，既程序崩溃，
    //打印现象为槽函数触发了两次，绘制函数触发一次，故使用中断处理。但是正常情况下子线程要考虑音视频同步，会比主线程慢，既在发送信号前延时可观察到程序并不会崩溃
    connect(&VideoDecoder::getInstall(), &VideoDecoder::frameDecoded, ui->displayWidget, &DisplayWidget::onFrameDecoded, Qt::DirectConnection);
    connect(_player, &Player::SyncTime, this, [this](double value){
        int s = round(value);
        ui->currentSlider->setValue(s);
        ui->currentLab->setText(getTimeText(s));
        //播放完毕停止
        if (s == ui->currentSlider->maximum()) {
            //qDebug() << ui->currentSlider->maximum();
            on_stopBtn_clicked();
        }
    });
    connect(ui->currentSlider, &videoSlider::clicked, this, [this](videoSlider* slider){
        _player->seekTime(slider->value());
    });
    connect(_volumeWidget, &Volume::changeVolume, &AudioDecoder::getInstall(), [this](float volume) {
        if (volume == 0) {
            setSilenceUI();
        }
        else {
            setHasVolumeUI();
        }
        AudioDecoder::getInstall().changeVolume(volume);
    });
}

MainWidget::~MainWidget()
{
    delete ui;
}

bool MainWidget::event(QEvent *event)
{
    //按下左右键改变进度
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Left) {
            int value = ui->currentSlider->value();
            value = (value < 5) ? 0 : value - 5;
            _player->seekTime(value);
        }
        else if (keyEvent->key() == Qt::Key_Right) {
            int maxValue = ui->currentSlider->maximum();
            int value = ui->currentSlider->value();
            if ((value + 5) < maxValue) value += 5;
            _player->seekTime(value);
        }
    }
    return QWidget::event(event);
}

bool MainWidget::eventFilter(QObject *watched, QEvent *e)
{
    if (watched == ui->volumeBtn) {
        if (e->type() == QMouseEvent::Enter){
            //获取到控件在全局的位置来移动音量窗口的位置
            _volumeWidget->move(ui->volumeBtn->mapToGlobal(QPoint(0,0)).x(), ui->volumeBtn->mapToGlobal(QPoint(0,0)).y() - 121);
            _volumeWidget->show();
        }
        if (e->type() == QMouseEvent::Leave){
            //鼠标不在音量窗口上关闭音量窗体
            if (!_volumeWidget->geometry().contains(QCursor::pos())){
                _volumeWidget->hide();
            }
        }
        //疑点，按钮安装了事件过滤器后信号槽失去了作用
        if (e->type() == QMouseEvent::MouseButtonPress) {
            if (_volumeBtnChecked) {
                //静音
                setSilenceUI();
                _volumeWidget->setSilence();
            }
            else {
                //解除静音
                setHasVolumeUI();
                _volumeWidget->recoverVolume();
            }
        }
    }
    if (watched == _volumeWidget) {
        if (e->type() == QMouseEvent::Leave)
            _volumeWidget->hide();
    }

    //监听滑块
    if (watched == ui->currentSlider && _preview->isRunning()) {
        if (e->type() == QMouseEvent::Enter) {
            //qDebug() << "mouse enter";
            _preview->show();
        }
        if (e->type() == QMouseEvent::Leave) {
            //qDebug() << "mouse Leave";
            _preview->hide();
        }
        if (e->type() == QMouseEvent::MouseMove) {
            //qDebug() << "mouse moving";
            QMouseEvent* me = static_cast<QMouseEvent*>(e);
            int position = ui->currentSlider->getSliderMovePoint(me->pos());

            int previewX = me->globalX() - (_preview->width() >> 1);
            int previewY = me->globalY() - _preview->height() - (ui->currentSlider->height() >> 1);
            static int lastHeight = previewY;
            //变化不大则固定高度
            if (abs(lastHeight - previewY) < 20) {
                previewY = lastHeight;
            } else lastHeight = previewY;
            _preview->move(mapFromGlobal(QPoint(previewX, previewY)));
            _preview->seek(position);

        }
    }
    return QWidget::eventFilter(watched,e);
}

void MainWidget::onStateChanged(State state)
{
    if (state == State::PLAYING) ui->playBtn->setText("暂停");
    else if (state == State::PAUSE) ui->playBtn->setText("播放");
    if (state == State::STOP) {
        ui->playBtn->setEnabled(false);
        ui->stopBtn->setEnabled(false);
        ui->currentSlider->setValue(0);
        ui->currentSlider->setEnabled(false);
        ui->currentLab->setText(getTimeText(0));
        ui->durationLab->setText(getTimeText(0));
        ui->displayWidget->free();
        _preview->Stop();
        //ui->volumeBtn->setEnabled(false);
    }
    else {
        ui->playBtn->setEnabled(true);
        ui->stopBtn->setEnabled(true);
        ui->currentSlider->setEnabled(true);
        //ui->volumeBtn->setEnabled(true);
    }
}

void MainWidget::onInitFinished()
{
    //设置进度条时间
    int duration = _player->getDurationTime();
    ui->currentSlider->setRange(0, duration);
    ui->durationLab->setText(getTimeText(duration));

    //预览窗口准备
    if (_player->hasVideo()) {
        _preview->Start(_fileName);
    }
}

void MainWidget::on_openFileBtn_clicked()
{
    QString filename = (ui->fileCombox->currentIndex() != 0) ? ui->fileCombox->currentText() :
                                                                QFileDialog::getOpenFileName(nullptr, "选择文件", "C:/Users/admin/Videos/material");
    if (filename.isEmpty()) return;
    _fileName = filename;
    _player->setFileName(filename);
    _player->play();
}


void MainWidget::on_playBtn_clicked()
{
    State state = _player->getState();
    if (state == State::PLAYING) _player->pause();
    else _player->play();
}

void MainWidget::on_stopBtn_clicked()
{
    _player->stop();
}

void MainWidget::setSilenceUI()
{
    ui->volumeBtn->setText(QChar(0xea0c));
    _volumeBtnChecked = false;
}

void MainWidget::setHasVolumeUI()
{
    ui->volumeBtn->setText(QChar(0xea0d));
    _volumeBtnChecked = true;
}

QString MainWidget::getTimeText(int value)
{
    QString h = QString("0%1").arg(value / 3600).right(2);
    QString m = QString("0%1").arg((value % 3600) / 60).right(2);
    QString s = QString("0%1").arg(value % 60).right(2);
    return QString("%1:%2:%3").arg(h).arg(m).arg(s);
}

