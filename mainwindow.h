#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPalette>
#include <QListWidgetItem>
#include <QDebug>
#include <QTimer>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QIcon>
#include <QScrollBar>
#include <QPainter>
#include <QModelIndex>
#include <QTableWidgetItem>
#include <QCoreApplication>
#include <QGraphicsDropShadowEffect>
#include <QPicture>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void saveToDo();
    void loadToDo();

    ///鼠标移动窗口
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    //移动窗口
    void mouseReleaseEvent(QMouseEvent *ev);
    //释放窗口


    ///窗口靠边自动隐藏

    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

    void keyPressEvent(QKeyEvent *event);
    bool event(QEvent *e);

    ///添加新条目
    void addNewItem(QString str);

    void paintEvent(QPaintEvent *event);

    bool eventFilter(QObject *obj, QEvent *event);

    void resizeEvent(QResizeEvent* event);

    ///获取显示器分辨率大小
    QDesktopWidget* desktopWidget;
    QRect screenRect;

public slots:

    void showWindow();
    void checkEmpty();
    void editingFunction();

Q_SIGNALS:
    void sig_focusChanged();

private slots:

    void on_addToDo_clicked();

    void on_toDoTable_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void on_toDoTable_itemDoubleClicked(QTableWidgetItem *item);

    void on_toDoTable_cellDoubleClicked(int row, int column);

    void on_toDoTable_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    //鼠标原始位置
    QPoint posMouseOrigin;

    ///出现的延迟时间
    int delayTime;



    QListWidgetItem *currentItem;

    bool checking = false;
    bool pinState = false;

    QIcon checkIcon;

    ///all todo item
    QStringList toDoList;

    int toDoColum = 1;
};

class SignalItemDelegate : public QStyledItemDelegate{
    Q_OBJECT
    Q_DISABLE_COPY(SignalItemDelegate)
public:
    explicit SignalItemDelegate(QObject* parent = Q_NULLPTR):QStyledItemDelegate(parent){
        QObject::connect(this,&SignalItemDelegate::closeEditor,this,&SignalItemDelegate::editFinished);
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const //Q_DECL_OVERRIDE
    {
        QStyledItemDelegate::setEditorData(editor, index);
        ///发送信号
        editStarted();

        ///取消文本框全选
        QLineEdit* le = qobject_cast<QLineEdit*>(editor);
        QObject src;
        connect(&src, &QObject::destroyed, le, [le](){
                            //set default selection in the line edit
                            int lastDotIndex= le->text().length();
                            le->setCursorPosition(lastDotIndex);
                        }, Qt::QueuedConnection);

        return QStyledItemDelegate::setEditorData(editor,index);
    }
    static bool isE;
Q_SIGNALS:
    void editStarted() const;
    void editFinished();
};



#endif // MAINWINDOW_H
