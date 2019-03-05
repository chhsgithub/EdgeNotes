#include "mainwindow.h"
#include "ui_mainwindow.h"
bool SignalItemDelegate::isE;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //setWindowOpacity(0.7); //透明

    ///setting event
    ui->setLabel->installEventFilter(this);
    setting = new Settings(this);
    connect(setting, &Settings::adjustSignal, this, &MainWindow::settingClosed);

    ///图标
    //checkIcon = QIcon(":/icons/check");
    checkIcon.addFile(":/icons/check");


    ///获取可用桌面大小
    desktopWidget = QApplication::desktop();
    screenRect = desktopWidget->screenGeometry();

    ///load to do item
    loadToDo();
    ///去除工具栏
    ui->mainToolBar->hide();


    ///透明
    this->setAttribute(Qt::WA_TranslucentBackground);

    delayTime = 500;


    ///判定是否在输入状态
    SignalItemDelegate* delegate = new SignalItemDelegate(ui->toDoTable);
    connect(delegate,&SignalItemDelegate::editStarted, this, &MainWindow::editingFunction);
    //QObject::connect(delegate,&SignalItemDelegate::editFinished,[](){SignalItemDelegate::isE = false;});
    connect(delegate,&SignalItemDelegate::editFinished, this, &MainWindow::checkEmpty);
    ui->toDoTable->setItemDelegate(delegate);

    ///todo table
    QStringList header;
    ui->toDoTable->setWordWrap(true);

    ///列设置
    ui->toDoTable->horizontalHeader()->hide();
    ui->toDoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->toDoTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);//对第0列单独设置固定宽度
    ui->toDoTable->setColumnWidth(0,30);
    ui->toDoTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);//对第0列单独设置固定宽度
    ui->toDoTable->setColumnWidth(2,30);
    ui->toDoTable->verticalHeader()->setVisible(false);
    ui->toDoTable->setShowGrid(false);
    ui->toDoTable->setFocusPolicy(Qt::NoFocus);

    //ui->toDoTable->setFocusPolicy();
    //ui->toDoTable->setEditTriggers(QAbstractItemView::CurrentChanged);
    //ui->toDoTable->horizontalHeader()->setSectionResizeMode (QHeaderView::ResizeToContents);
    //ui->toDoTable->horizontalHeader()->setMaximumSectionSize(int);
    //ui->toDoTable->setTextElideMode(Qt::ElideNone);
    //ui->toDoTable->resizeRowsToContents();

    ///talbe鼠标事件
    ui->toDoTable->viewport()->installEventFilter(this);

    ///table shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
    effect->setOffset(0);
    effect->setBlurRadius(10);
    ui->toDoTable->setGraphicsEffect(effect);


    ///pin or unpin event
    ui->pinLabel->installEventFilter(this);

    ///load setting
    settingClosed();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    //创建绘画对象
    QPainter painter(this);
    //为窗口添加一个半透明的矩形遮罩
    painter.fillRect(this->rect(),QColor(0,0,0,1));
    ui->toDoTable->resizeRowsToContents();
}

//获取鼠标的按下操作
void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    this->posMouseOrigin = QCursor::pos(); //cursor是一个光标类
    qDebug()<< this->posMouseOrigin;

}

//获得鼠标移动的操作
void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint ptMouseNow = QCursor::pos();
    QPoint ptDelta = ptMouseNow - this->posMouseOrigin;
    move(this->pos() + ptDelta);
    posMouseOrigin = ptMouseNow;
}

//获得鼠标的释放操作；
void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{

}
//鼠标事件
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->toDoTable)
    {
        if (event->type() == QEvent::MouseButtonPress){
            //qDebug() << "table mouse press event";
        }
        else if (event->type() == QEvent::MouseMove){
            //qDebug() << "table mouse moveevent";
        }
    }
    else if (obj == ui->toDoTable->viewport())
    {
        if (event->type() == QEvent::MouseButtonPress){
            //qDebug() << "table->viewport mouse press event";
            this->posMouseOrigin = QCursor::pos(); //cursor是一个光标类
        }
        else if (event->type() == QEvent::MouseMove){
            QPoint ptMouseNow = QCursor::pos();
            QPoint ptDelta = ptMouseNow - this->posMouseOrigin;
            move(this->pos() + ptDelta);
            posMouseOrigin = ptMouseNow;
            //qDebug() << "table->viewport mouse moveevent";
        }
    }

    ///click pin lable
    if(obj == ui->pinLabel && event->type() == QEvent::MouseButtonPress){
        qDebug()<<"click pinlabel";
        pinState = !pinState;
        if(pinState){
            qDebug()<<"unpin";
            ui->pinLabel->setPixmap(QPixmap(":/icons/unpin"));
        }
        else{
            ui->pinLabel->setPixmap(QPixmap(":/icons/pin"));
        }
    }

    if(obj == ui->setLabel && event->type() == QEvent::MouseButtonPress){
        qDebug()<<"click setLabel";
        tempPinState = pinState;
        pinState = true;
        setting->show();
        QRect rect;
        rect = setting->geometry();
        setting->move((screenRect.width()- rect.width())/2,
                      (screenRect.height()- rect.height())/2);
    }

    return QMainWindow::eventFilter(obj, event);
}

///进入计时，计时结束显示窗口
void MainWindow::enterEvent(QEvent *)
{
    QRect rect;
    rect = this->geometry();
    if(rect.top()<0)
    {
        QTimer::singleShot(delayTime, this, SLOT(showWindow()));

    }
}

///显示窗口
void MainWindow::showWindow(){
    ui->addToDo->setFocus();
    QRect rect;
    rect = this->geometry();
    ///判定鼠标是否还在角落
    QPoint ptMouseNow = QCursor::pos();
    if (ptMouseNow.x() > (screenRect.width() - 3) && ptMouseNow.y() < 2){
        ///(x,y)
        move(screenRect.width() - rect.width(), 0);
    }
}

///离开窗口
void MainWindow::leaveEvent(QEvent *)
{
    if(SignalItemDelegate::isE || pinState){
        return;
    }
    QRect rect;
    rect = this->geometry();
    move(screenRect.width()- 1, -rect.height()+1);

}

///窗口被隐藏
bool MainWindow::event(QEvent *e)
{
    // window was deactivated
    if (e->type() == QEvent::WindowDeactivate && !pinState) {
        SignalItemDelegate::isE = false;
        QRect rect;
        rect = this->geometry();
        move(screenRect.width()- 1, -rect.height()+1);
    }
    return QWidget::event(e);
}


void MainWindow::on_addToDo_clicked()
{
    addNewItem("add button");
}

///添加新条目
void MainWindow::addNewItem(QString str)
{
    ///talbe 操作
    /// 插入一行
    ui->toDoTable->insertRow(ui->toDoTable->rowCount());
    int rowCount = ui->toDoTable->rowCount();

    /// 插入空字符
    QTableWidgetItem *toDoItem = new QTableWidgetItem;
    toDoItem->setText(str);
    ui->toDoTable->setItem(rowCount - 1, 1, toDoItem);

    ///插入新行空图标
    QTableWidgetItem *emptyItem = new QTableWidgetItem;
    emptyItem->setFlags(emptyItem->flags() ^ Qt::ItemIsEditable);
    ui->toDoTable->setItem(rowCount - 1, 0, emptyItem);
    qDebug()<<ui->toDoTable->rowCount();

    /// 插入check 图标
    if(rowCount < 2){
        return;
    }
    QTableWidgetItem *check = new QTableWidgetItem;
    check->setIcon(checkIcon);
    check->setFlags(check->flags() ^ Qt::ItemIsEditable);
    ui->toDoTable->setItem(rowCount - 2, 0, check);


    ///save to local
}


void MainWindow::keyPressEvent(QKeyEvent *event) //重写keyPressEvent函数
{
    int key = event->key();
    if(key==Qt::Key_Enter || (key==Qt::Key_Return)){

    }
    qDebug()<<event->type();
    qDebug()<<SignalItemDelegate::isE;
}



void MainWindow::checkEmpty()
{   if(checking){
        return;
    }
    checking = true;
    qDebug() << "edit finish";
    SignalItemDelegate::isE = false;

    ///talbe操作
    QString itemString;
    int toDoCount = ui->toDoTable->rowCount();
    for(int i = 0; i< toDoCount - 1; i++){
        itemString = ui->toDoTable->item(i, 1)->text();
        if(itemString == ""){
            ui->toDoTable->removeRow(i);
            i--;
            toDoCount--;
        }
    }

    //toDoCount = ui->toDoTable->rowCount();
    //itemString = ui->toDoTable->item(toDoCount-1, 1)->text();
    if(itemString != ""){
        //addNewItem("");
    }

    ///save to loacl
    saveToDo();
    checking = false;
}

void MainWindow::editingFunction(){
    qDebug()<<"editing";
    ui->toDoTable->resizeRowsToContents();
    SignalItemDelegate::isE = true;
}

void MainWindow::on_toDoTable_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    qDebug()<< "on_toDoTable_currentItemChanged";
    //qDebug()<< ui->toDoTable->item(current->row(), 1)->text();
    SignalItemDelegate::isE = false;
    //addNewItem("");

    checkEmpty();
    ui->toDoTable->resizeRowsToContents();
}

void MainWindow::on_toDoTable_itemDoubleClicked(QTableWidgetItem *item)
{
    //qDebug()<<"on_toDoTable_itemDoubleClicked";
    //int Lindex = ui->toDoView->currentRow();

}

void MainWindow::on_toDoTable_cellDoubleClicked(int row, int column)
{
    qDebug()<<"on_toDoTable_cellDoubleClicked";
    int rowCount = ui->toDoTable->rowCount();
    //qDebug() << index.row() << num;
    ///双击todo项目
    if(row == rowCount-1 && column == 1){
        addNewItem("");
    }
    ///双击check
    if(row != rowCount - 1 && column == 0){
        ui->toDoTable->removeRow(row);
        saveToDo();
    }
}

void MainWindow::on_toDoTable_clicked(const QModelIndex &index)
{
    qDebug ()<<"click";
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   //qDebug()<<"resize";
   ui->toDoTable->resizeRowsToContents();
}

void MainWindow::saveToDo()
{
    ///get all todo item to QStringList
    toDoList.clear();
    int currentNum = ui->toDoTable->rowCount();
    for(int i = 0; i < currentNum - 1; i++){
        toDoList<<ui->toDoTable->item(i, toDoColum)->text();
    }
    qDebug()<<"save todo " << toDoList;

    //save user file
    //qRegisterMetaTypeStreamOperators<QStringList>("toDoList");
    QString fileName = "cookies";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug()<<"fail file";
             return;
     }
     QDataStream out(&file);
     out.setVersion(QDataStream::Qt_4_8);
     out << toDoList;
     file.close();
}

void MainWindow::loadToDo(){
    QString fileName = "cookies";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"fail file";
        addNewItem("double click to add your first task");
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);
    in >> toDoList;
    qDebug()<<"todo is "<<toDoList;
    for(int i = 0; i< toDoList.size(); i++){
        addNewItem(toDoList[i]);
    }
    if(toDoList.size() == 0 ){
        addNewItem("double click to add your first task");
    }
    addNewItem("");
}

void MainWindow::settingClosed(){
    pinState = tempPinState;
    delayTime = setting->appearTime;
    ui->toDoTable->setFont(setting->fnt);
    ui->toDoTable->resizeRowsToContents();
}
