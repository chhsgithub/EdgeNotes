#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QObject>
#include <QtDebug>
#include <QGraphicsDropShadowEffect>

namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    ~settings();
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    Ui::settings *ui;

    ///mouse position
    QPoint posMouseOrigin;

signals:
    void closeSignal();//这个函数用户向主界面通知关闭的消息

};

#endif // SETTINGS_H
