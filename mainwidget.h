#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QString>
#include "player.h"
#include "volume.h"

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
    bool eventFilter(QObject *watched, QEvent *e) override;

private slots:
    void onStateChanged(State state);
    void onInitFinished();
    void on_openFileBtn_clicked();

    void on_playBtn_clicked();

    void on_stopBtn_clicked();

private:
    void setSilenceUI();
    void setHasVolumeUI();

private:
    QString getTimeText(int value);

    Ui::MainWidget *ui;
    Volume* _volumeWidget = nullptr;
    Player* _player;

    bool _volumeBtnChecked = true;        //按钮点击变量
};
#endif // MAINWIDGET_H
