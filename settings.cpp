#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool |Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint);

    ///透明
    this->setAttribute(Qt::WA_TranslucentBackground);

    ///table shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
    effect->setOffset(0);
    effect->setBlurRadius(10);
    ui->settingTable->setGraphicsEffect(effect);

    ///table setting
    ui->settingTable->horizontalHeader()->hide();
    ui->settingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->settingTable->verticalHeader()->setVisible(false);
    ui->settingTable->setShowGrid(false);
    ui->settingTable->setFocusPolicy(Qt::NoFocus);

    ///init
    ui->settingTable->setRowCount(3);
    QTableWidgetItem *font = new QTableWidgetItem;
    font->setText("font");
    font->setFlags(font->flags() ^ Qt::ItemIsEditable);
    ui->settingTable->setItem(0, 0, font);

    QTableWidgetItem *fontSize =new QTableWidgetItem;
    fontSize->setText("font size");
    fontSize->setFlags(fontSize->flags() ^ Qt::ItemIsEditable);
    ui->settingTable->setItem(1, 0, fontSize);

    QTableWidgetItem *appearTime = new QTableWidgetItem;
    appearTime->setText("appear time");
    appearTime->setFlags(appearTime->flags() ^ Qt::ItemIsEditable);
    ui->settingTable->setItem(2, 0, appearTime);

    //ui->settingTable->viewport()->installEventFilter(this);
}

settings::~settings()
{
    delete ui;
}

void settings::on_pushButton_clicked()
{
    emit closeSignal();
    this->close();
}

//鼠标事件
bool settings::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->settingTable)
    {
        if (event->type() == QEvent::MouseButtonPress){
            //qDebug() << "table mouse press event";
        }
        else if (event->type() == QEvent::MouseMove){
            //qDebug() << "table mouse moveevent";
        }
    }
    else if (obj == ui->settingTable->viewport())
    {
        if (event->type() == QEvent::MouseButtonPress){
            qDebug() << "table->viewport mouse press event";
            this->posMouseOrigin = QCursor::pos(); //cursor是一个光标类
        }
        else if (event->type() == QEvent::MouseMove){
            QPoint ptMouseNow = QCursor::pos();
            QPoint ptDelta = ptMouseNow - this->posMouseOrigin;
            move(this->pos() + ptDelta);
            posMouseOrigin = ptMouseNow;
            qDebug() << "table->viewport mouse moveevent";
        }
    }

    return settings::eventFilter(obj, event);
}
