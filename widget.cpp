//     Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    globalInit();
    myCom = NULL;
    isFirst = true;
    ui->recieveTextBrowser->setOpenLinks(false);
    connect(ui->recieveTextBrowser, SIGNAL(anchorClicked(const QUrl&)),this, SLOT(anchorClickedSlot(const QUrl&)));

    QString date = QLocale(QLocale::C).toDate(QString(__DATE__).replace("  "," "),"MMM d yyyy").toString("yyyy-MM-dd");
    QString time = QString(__TIME__).left(5);
    setWindowTitle(tr("编译于 ")+date+" "+time);

    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->setText(tr("欢迎使用Spacelan's 串口调试助手！\n\nSpacelan's 串口调试助手是Spacelan写的第一个也是目前唯一一个有那么一丢丢实用价值的QT程序\n\nSpacelan's 串口调试助手是某个项目的基础程序\n\nSpacelan's 串口调试助手的代码参考了开源程序QCom"));
    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->insertHtml(tr("<a href=\"http://www.qter.org/?page_id=203\">参考链接</a><br><br>"));
    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->insertPlainText("GitHub ");
    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->insertHtml("<a href=\"https://github.com/spacelan/MyCom.git\">https://github.com/spacelan/MyCom.git</a><br><br>");
    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->insertPlainText("Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.");
    ui->recieveTextBrowser->moveCursor(QTextCursor::Start);

    ui->portnameComboBox->setCurrentIndex(3);
    ui->baudRateComboBox->setCurrentIndex(1);

    isGLOpened = false;
    connect(&w,SIGNAL(closeWidget()),this,SLOT(on_displayBTN_clicked()));
//    connect(this,SIGNAL(sendQuatData(float,float,float,float)),&w,SLOT(quatToMatrix(float,float,float,float)));
}

Widget::~Widget()
{
    if(myCom != NULL)
    {
        if(myCom->isOpen())
            myCom->close();
        delete myCom;
    }
    delete ui;
}

void Widget::globalInit()
{
    ui->openclosebtn->setText(tr("打开串口"));
    ui->sendbtn->setText(tr("发送"));
    ui->sendbtn->setEnabled(false);
    ui->runpauseBtn->setEnabled(false);
    ui->restartBtn->setEnabled(false);
}

void Widget::setComboxEnabled(bool b)
{
    ui->portnameComboBox->setEnabled(b);
    ui->baudRateComboBox->setEnabled(b);
    ui->dataBitsComboBox->setEnabled(b);
    ui->parityComboBox->setEnabled(b);
    ui->stopbitsComboBox->setEnabled(b);
}

void Widget::closeEvent(QCloseEvent *e)
{
    if(isGLOpened)
        w.close();
    e->accept();
}

void Widget::mySetText(float &w,float &x,float &y,float &z,float &pitch,float &roll,float &yaw)
{
    QString str;
    str.sprintf("%f %f %f %f %f %f %f\n",
                w,x,y,z,pitch,roll,yaw);
    ui->recieveTextBrowser->setTextColor(Qt::black);
    ui->recieveTextBrowser->moveCursor(QTextCursor::End);
    ui->recieveTextBrowser->insertPlainText(str);

    str.sprintf("%f",w);
    ui->label_w_display->setText(str);
    str.sprintf("%f",x);
    ui->label_x_display->setText(str);
    str.sprintf("%f",y);
    ui->label_y_display->setText(str);
    str.sprintf("%f",z);
    ui->label_z_display->setText(str);
    str.sprintf("%f",pitch);
    ui->label_pitch_display->setText(str);
    str.sprintf("%f",roll);
    ui->label_roll_display->setText(str);
    str.sprintf("%f",yaw);
    ui->label_yaw_display->setText(str);

    this->w.quatToMatrix(w,x,y,z);
}

//every   ms
void Widget::readMyCom()
{
    //未打开串口或者不能读取
    if(!(myCom && myCom->isReadable())) return;

    QByteArray buf = myCom->readAll();
    mySerialData->handleTheBuf(buf);
}

void Widget::on_openclosebtn_clicked()
{
    //启动后起一次打开，清除屏幕
    if(isFirst)
    {
        ui->recieveTextBrowser->clear();
        isFirst = false;
    }

    //如果已经开启，关闭串口
    if(myCom)
    {
        myCom->close();
        delete myCom;
        myCom =NULL;

        delete myTimer;
        delete mySerialData;

        ui->appName->setText(tr("未打开..."));
        ui->sendbtn->setEnabled(false);
        setComboxEnabled(true);
        ui->openclosebtn->setText(tr("打开串口"));

        ui->runpauseBtn->setEnabled(false);
        ui->restartBtn->setEnabled(false);

//        QMessageBox::information(this,tr("关闭串口成功"),tr("成功关闭串口")/* + portName*/,QMessageBox::Ok);
        return;
    }

    //打开串口准备
    mySerialData = new handleserialdata;
    QString portName = ui->portnameComboBox->currentText();
    myCom = new QextSerialPort(portName);

    myTimer = new QTimer;
    myTimer->setInterval(50);
    myTimer->start();
    connect(myTimer,SIGNAL(timeout()),this,SLOT(readMyCom()));

    connect(mySerialData,SIGNAL(sendHandledData(float&,float&,float&,float&,float&,float&,float&)),this,SLOT(mySetText(float&,float&,float&,float&,float&,float&,float&)));


    //配置串口
    myCom->setBaudRate((BaudRateType)ui->baudRateComboBox->currentText().toInt());
    switch(ui->dataBitsComboBox->currentIndex())
    {
    case 0:
        myCom->setDataBits(DATA_8);
        break;
    case 1:
        myCom->setDataBits(DATA_7);
        break;
    default:
        myCom->setDataBits(DATA_8);
    }
    switch(ui->parityComboBox->currentIndex())
    {
    case 0:
        myCom->setParity(PAR_NONE);
        break;
    case 1:
        myCom->setParity(PAR_ODD);
        break;
    case 2:
        myCom->setParity(PAR_EVEN);
        break;
    default:
        myCom->setParity(PAR_NONE);
    }
    switch(ui->stopbitsComboBox->currentIndex())
    {
    case 0:
        myCom->setStopBits(STOP_1);
        break;
    case 1:
        myCom->setStopBits(STOP_1_5);
        break;
    case 2:
        myCom->setStopBits(STOP_2);
        break;
    default:
        myCom->setStopBits(STOP_1);
    }
    myCom->setFlowControl(FLOW_OFF);
    myCom->setTimeout(TIME_OUT);

    //打开串口
    if(myCom->open(QIODevice::ReadWrite))
    {

        //界面控制
        ui->appName->setText(tr("已打开..."));
        ui->openclosebtn->setText(tr("关闭串口"));
        ui->sendbtn->setEnabled(true);
        setComboxEnabled(false);
        ui->sendLineEdit->setFocus();

        ui->runpauseBtn->setEnabled(true);
        ui->restartBtn->setEnabled(true);

//        QMessageBox::information(this,tr("打开串口成功"),tr("成功打开串口")+portName,QMessageBox::Ok);

    }
    else
    {
        QMessageBox::critical(this, tr("打开失败"), tr("未能打开串口 ") + portName + tr("\n该串口设备不存在或已被占用"), QMessageBox::Ok);
        return;
    }
}

void Widget::on_sendbtn_clicked()
{
    if(ui->sendLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("提示消息"), tr("没有需要发送的数据"), QMessageBox::Ok);
        return;
    }
    ui->sendLineEdit->setFocus();
    QByteArray buf;
    buf = ui->sendLineEdit->text().toAscii();
    myCom->write(buf);
    ui->recieveTextBrowser->setTextColor(Qt::lightGray);
}

void Widget::anchorClickedSlot(const QUrl &url)
{
    ShellExecuteA(NULL, "open", url.toString().toStdString().c_str(), "", "", SW_SHOW);
}

void Widget::on_clearbtn_clicked()
{
    ui->recieveTextBrowser->clear();
}

void Widget::on_restartBtn_clicked()
{
    QByteArray buf;
    buf[0] = 's'; //buf[1] = 0x55; buf[2] = 0x01; buf[3] = myDataType_Restart;
    myCom->write(buf);
}

void Widget::on_runpauseBtn_clicked()
{
    static bool run = false;
    QByteArray buf;
    if(run)
    {
        buf[0] = 'p'; //buf[1] = 0x55; buf[2] = 0x01; buf[3] = myDataType_Pause;
        myCom->write(buf);
        ui->runpauseBtn->setText("run");
        run = false;
    }
    else
    {
        buf[0] = 'r'; //buf[1] = 0x55; buf[2] = 0x01; buf[3] = myDataType_Run;
        myCom->write(buf);
        ui->runpauseBtn->setText("pause");
        run = true;
    }
}

void Widget::on_displayBTN_clicked()
{
    if(!isGLOpened)
    {
        w.show();
        ui->displayBTN->setText(tr("关闭动画"));
        isGLOpened = true;
    }
    else
    {
        w.close();
        ui->displayBTN->setText(tr("打开动画"));
        isGLOpened = false;
    }
}
