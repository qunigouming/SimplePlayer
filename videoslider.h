#ifndef VIDEOSLIDER_H
#define VIDEOSLIDER_H

#include <QSlider>
#include "previewwidget.h"

//该类用于操作进度条
class videoSlider : public QSlider
{
    Q_OBJECT
public:
    explicit videoSlider(QWidget* parent = nullptr);
    int getSliderMovePoint(const QPoint& pos);       //获取当前鼠标在滑动条上的值

signals:
    void clicked(videoSlider* slider);

protected:
    void mousePressEvent(QMouseEvent* e) override;
    // void mouseMoveEvent(QMouseEvent *me) override;
    // void leaveEvent(QEvent *e) override;
    // void enterEvent(QEvent *e) override;
};

#endif // VIDEOSLIDER_H
