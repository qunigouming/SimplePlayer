#include "volume.h"
#include "ui_volume.h"

Volume::Volume(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Volume)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
}

Volume::~Volume()
{
    delete ui;
}

void Volume::setSilence()
{
    _volume = ui->volumeSlider->value();
    ui->volumeSlider->setValue(0);
}

void Volume::recoverVolume()
{
    ui->volumeSlider->setValue(_volume);
}

void Volume::on_volumeSlider_valueChanged(int value)
{
    ui->volumeLab->setText(QString("%1%").arg(value));
    emit changeVolume(static_cast<float>(value/100.0));
}

