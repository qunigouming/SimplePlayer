#include "videoslider.h"
#include <QMouseEvent>

videoSlider::videoSlider(QWidget *parent) : QSlider(parent) {

}

void videoSlider::mousePressEvent(QMouseEvent *e)
{
    int ratio = (e->pos().x() * 1.0 / width()) * (maximum() - minimum());
    setValue(ratio);

    QSlider::mousePressEvent(e);

    emit clicked(this);
}
