#ifndef VIDEOSLIDER_H
#define VIDEOSLIDER_H

#include <QSlider>

//该类用于操作进度条
class videoSlider : public QSlider
{
    Q_OBJECT
public:
    videoSlider(QWidget* parent = nullptr);

signals:
    void clicked(videoSlider* slider);

private:
    void mousePressEvent(QMouseEvent* e) override;
};

#endif // VIDEOSLIDER_H
