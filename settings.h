#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QObject>
#include <QtDebug>
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <SignalItemDelegate.h>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    QFont fnt;
    //uint font;
    uint fontSize;
    int appearTime;
    QString account;

public slots:

    void startEdit(){}
    void endEdit();

private slots:
    void on_pushButton_clicked();

    void on_resetButton_clicked();

private:
    Ui::Settings *ui;

    bool eventFilter(QObject *obj, QEvent *event);
    ///mouse position
    QPoint posMouseOrigin;
    QStringList configs;

    void saveSetting();
    bool loadSetting();

    void adjustLayout();

signals:
    void adjustSignal();//这个函数用户向主界面通知关闭的消息

};

#endif // SETTINGS_H
