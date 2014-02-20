//     Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QMessageBox>
#include <qextserialport/qextserialport.h>
#include <QTimer>
#include <windows.h>
#include "handleserialdata.h"
#include <QCloseEvent>
#include "nehewidget/nehewidget.h"

//#include <qextserialport_p.h>
//#include <qextserialport_global.h>

//串口读写延时
#define TIME_OUT 10

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:

    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
    //全局初始化
    void globalInit();
    void setComboxEnabled(bool);
    void closeEvent(QCloseEvent *e);

private:
    Ui::Widget *ui;
    bool isFirst;
    QextSerialPort *myCom;
    QTimer *myTimer;
    handleserialdata *mySerialData;
    NeHeWidget w;
    bool isGLOpened;

private slots:
    void mySetText(float &w,float &x,float &y,float &z,float &pitch,float &roll,float &yaw);
    void readMyCom();
    void on_openclosebtn_clicked();
    void on_sendbtn_clicked();
    void anchorClickedSlot(const QUrl&);
    void on_clearbtn_clicked();
    void on_runpauseBtn_clicked();
    void on_restartBtn_clicked();
    void on_displayBTN_clicked();

signals:
    void sendQuatData(float w,float x,float y,float z);
};

#endif // WIDGET_H
