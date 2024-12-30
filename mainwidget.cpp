#include "mainwidget.h"
#include "./ui_mainwidget.h"
#include <QFileDialog>
#include "displaywidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    _player = new Player(this);
    ui->fileCombox->addItem(QString());
    ui->fileCombox->addItem("https://vs-cmaf-push-ww-live.akamaized.net/x=4/i=urn:bbc:pips:service:bbc_news_channel_hd/iptv_hd_abr_v1.mpd");
    ui->fileCombox->addItem("rtmp://192.168.186.130:1935/live1");
    ui->fileCombox->setCurrentIndex(0);
    connect(_player, &Player::stateChanged, this, &MainWidget::onStateChanged);
    connect(_player, &Player::initFinished, this, &MainWidget::onInitFinished);
    //connect第五个参数不填默认为AutoConnection，既两者同线程下用DirectConnection，异线程用QueuedConnection，使用事件队列处理信号槽，可能会导致OpenGL访问空指针问题，既程序崩溃，
    //打印现象为槽函数触发了两次，绘制函数触发一次，故使用中断处理。但是正常情况下子线程要考虑音视频同步，会比主线程慢，既在发送信号前延时可观察到程序并不会崩溃
    connect(_player->getVideoPtr(), &VideoDecoder::frameDecoded, ui->displayWidget, &DisplayWidget::onFrameDecoded, Qt::DirectConnection);
    connect(_player, &Player::SyncTime, this, [this](double value){
        int s = round(value);
        ui->currentSlider->setValue(s);
        ui->currentLab->setText(getTimeText(s));
        //同步进度条时改变播放状态
        if (s == ui->currentSlider->maximum()) {
            qDebug() << ui->currentSlider->maximum();
            _player->pause();
        }
    });
    connect(ui->currentSlider, &videoSlider::clicked, this, [this](videoSlider* slider){
        _player->seekTime(slider->value());
    });
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::onStateChanged(State state)
{
    if (state == State::PLAYING) ui->playBtn->setText("暂停");
    else if (state == State::PAUSE) ui->playBtn->setText("播放");
    if (state == State::STOP) {
        ui->playBtn->setEnabled(false);
        ui->stopBtn->setEnabled(false);
        ui->currentSlider->setEnabled(false);
        ui->volumnBtn->setEnabled(false);
    }
    else {
        ui->playBtn->setEnabled(true);
        ui->stopBtn->setEnabled(true);
        ui->currentSlider->setEnabled(true);
        ui->volumnBtn->setEnabled(true);
    }
}

void MainWidget::onInitFinished()
{
    int duration = _player->getDurationTime();
    ui->currentSlider->setRange(0, duration);
    ui->durationLab->setText(getTimeText(duration));
}

void MainWidget::on_openFileBtn_clicked()
{
    QString filename = (ui->fileCombox->currentIndex() != 0) ? ui->fileCombox->currentText() :
                                                                QFileDialog::getOpenFileName(nullptr, "选择文件", "C:/Users/admin/Videos/material");
    if (filename.isEmpty()) return;
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
    ui->displayWidget->free();
}

QString MainWidget::getTimeText(int value)
{
    QString h = QString("0%1").arg(value / 3600).right(2);
    QString m = QString("0%1").arg((value % 3600) / 60).right(2);
    QString s = QString("0%1").arg(value % 60).right(2);
    return QString("%1:%2:%3").arg(h).arg(m).arg(s);
}

