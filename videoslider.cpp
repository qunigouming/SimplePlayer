#include "videoslider.h"
#include <QMouseEvent>
#include <QStyleOptionSlider>

videoSlider::videoSlider(QWidget *parent) : QSlider(parent) {
    setMouseTracking(true);
}

int videoSlider::getSliderMovePoint(const QPoint &pos)
{
    // 获取滑块样式信息
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    // 计算滑块可点击区域的矩形
    QRect grooveRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    // 根据方向调整有效区域
    int available;
    if (orientation() == Qt::Horizontal) {
        grooveRect.adjust(handleRect.width()/2, 0, -handleRect.width()/2, 0);
        available = grooveRect.width();
    } else {
        grooveRect.adjust(0, handleRect.height()/2, 0, -handleRect.height()/2);
        available = grooveRect.height();
    }

    // 计算鼠标位置对应的值
    int position;
    double normalizedPosition;
    if (orientation() == Qt::Horizontal) {
        position = qBound(0, pos.x() - grooveRect.x(), grooveRect.width());
        normalizedPosition = static_cast<double>(position) / available;
    } else {
        position = qBound(0, grooveRect.bottom() - pos.y(), grooveRect.height());
        normalizedPosition = static_cast<double>(position) / available;
    }

    // 计算实际值
    int value = minimum() + qRound(normalizedPosition * (maximum() - minimum()));
    return value;
}

void videoSlider::mousePressEvent(QMouseEvent *e)
{
    int ratio = (e->pos().x() * 1.0 / width()) * (maximum() - minimum());
    setValue(ratio);

    QSlider::mousePressEvent(e);

    emit clicked(this);
}

// void videoSlider::mouseMoveEvent(QMouseEvent *me)
// {
//     if (!_preview->isRunning()) {
//         QSlider::mouseMoveEvent(me);
//         return;
//     }
//     int position = getSliderMovePoint(me->pos());

//     int previewX = me->globalX() - (_preview->width() >> 1);
//     int previewY = me->globalY() - _preview->height() - (height() >> 1);
//     static int lastHeight = previewY;
//     //变化不大则固定高度
//     if (abs(lastHeight - previewY) < 20) {
//         previewY = lastHeight;
//     } else lastHeight = previewY;
//     _preview->move(mapFromGlobal(QPoint(previewX, previewY)));
//     _preview->seek(position);
//     QSlider::mouseMoveEvent(me);
// }

// void videoSlider::leaveEvent(QEvent *e)
// {
//     if (!_preview->isRunning()) {
//         QSlider::leaveEvent(e);
//         return;
//     }
//     qDebug() << "mouse Leave";
//     _preview->hide();
//     QSlider::leaveEvent(e);
// }

// void videoSlider::enterEvent(QEvent *e)
// {
//     if (!_preview->isRunning()) {
//         QSlider::enterEvent(e);
//         return;
//     }
//     qDebug() << "mouse enter";
//     _preview->show();
//     QSlider::enterEvent(e);
// }
