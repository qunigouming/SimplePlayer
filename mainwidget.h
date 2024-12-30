#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QString>
#include "player.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

protected:
    bool event(QEvent *event) override;

private slots:
    void onStateChanged(State state);
    void onInitFinished();
    void on_openFileBtn_clicked();

    void on_playBtn_clicked();

    void on_stopBtn_clicked();

private:
    QString getTimeText(int value);

    Ui::MainWidget *ui;
    Player* _player;
};
#endif // MAINWIDGET_H
