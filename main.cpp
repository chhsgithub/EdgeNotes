#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication a(argc, argv);

    MainWindow w;
    //QPalette wpal = w.palette();
    //wpal.setColor(QPalette::Background, QColor(100,100,100,50));
    //w.setPalette(wpal);
    //w.setAutoFillBackground(true);
    //w.setStyleSheet("background-color:rgba(244,244,244,2)");
    //w.setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    //w.setWindowFlags(Qt::Tool);
    ///无任务栏图标|无标题栏|永远置顶
    w.setWindowFlags(Qt::Tool |Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint);
    w.show();

    ///move mainwidow
    QRect rect;
    rect = w.geometry();
    w.move(w.screenRect.width()- 1, -rect.height()+1);
    return a.exec();
}
