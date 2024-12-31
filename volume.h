#ifndef VOLUME_H
#define VOLUME_H

#include <QDialog>

namespace Ui {
class Volume;
}

class Volume : public QDialog
{
    Q_OBJECT

public:
    explicit Volume(QWidget *parent = nullptr);
    ~Volume();

signals:
    void changeVolume(float volume);

public slots:
    void setSilence();         //设置静音
    void recoverVolume();           //恢复音量

private slots:
    void on_volumeSlider_valueChanged(int value);

private:
    float _volume = 100;
    Ui::Volume *ui;
};

#endif // VOLUME_H
