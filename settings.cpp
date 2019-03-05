#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
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

    QTableWidgetItem *fontSizeItem =new QTableWidgetItem;
    fontSizeItem->setText("font size");
    fontSizeItem->setFlags(fontSizeItem->flags() ^ Qt::ItemIsEditable);
    ui->settingTable->setItem(1, 0, fontSizeItem);

    QTableWidgetItem *appearTimeItem = new QTableWidgetItem;
    appearTimeItem->setText("appear time (ms)");
    appearTimeItem->setFlags(appearTimeItem->flags() ^ Qt::ItemIsEditable);
    ui->settingTable->setItem(2, 0, appearTimeItem);

    ///default settings
    if(!loadSetting()){
        on_resetButton_clicked();
    }

    ui->settingTable->setFont(fnt);
    //ui->settingTable->viewport()->installEventFilter(this);



    ///editing status
    SignalItemDelegate* delegate = new SignalItemDelegate(ui->settingTable);
    connect(delegate,&SignalItemDelegate::editStarted, this, &Settings::startEdit);
    connect(delegate,&SignalItemDelegate::editFinished, this, &Settings::endEdit);
    ui->settingTable->setItemDelegate(delegate);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pushButton_clicked()
{
    //emit adjustSignal();
    this->close();
}

//鼠标事件
bool Settings::eventFilter(QObject *obj, QEvent *event)
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

    return Settings::eventFilter(obj, event);
}

void Settings::endEdit(){
    fnt.setFamily(ui->settingTable->item(0,1)->text());
    fnt.setPointSize(ui->settingTable->item(1,1)->text().toInt());
    appearTime = ui->settingTable->item(2,1)->text().toInt();
    adjustLayout();
    saveSetting();
}

void Settings::saveSetting()
{
    configs.clear();
    for(int i = 0; i< ui->settingTable->rowCount(); i++){
        configs.append(ui->settingTable->item(i,1)->text());
    }
    //save user configs
    QString fileName = "configs";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug()<<"fail file";
        return;
     }
     QDataStream out(&file);
     out.setVersion(QDataStream::Qt_4_8);
     out << configs;
     file.close();
}

bool Settings::loadSetting()
{
    configs.clear();
    QString fileName = "configs";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"fail file";
        return false;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);
    in >> configs;
    fnt.setPointSize(configs[1].toInt());
    appearTime = configs[2].toInt();
    ///set configs in table
    for(int i = 0; i< ui->settingTable->rowCount(); i++){
        ui->settingTable->setItem(i, 1, new QTableWidgetItem(configs[i]));
    }
    emit adjustSignal();
    return true;
}

void Settings::adjustLayout(){
    ui->settingTable->setFont(fnt);
    ui->settingTable->setItem(0, 1, new QTableWidgetItem(fnt.family()));
    ui->settingTable->setItem(1, 1, new QTableWidgetItem(QString::number(fnt.pointSize())));
    ui->settingTable->setItem(2, 1, new QTableWidgetItem(QString::number(appearTime)));
    ui->settingTable->resizeRowsToContents();
    emit adjustSignal();
}

void Settings::on_resetButton_clicked()
{
    fnt.setFamily("Microsoft YaHei");
    fnt.setPointSize(18);
    appearTime = 500;
    adjustLayout();
    saveSetting();
}
