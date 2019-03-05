#ifndef SIGNALITEMDELEGATE_H
#define SIGNALITEMDELEGATE_H

#include <QCoreApplication>
#include <QStyledItemDelegate>
#include <QLineEdit>

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

#endif // SIGNALITEMDELEGATE_H
