//     Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.

#ifndef HANDLESERIALDATA_H
#define HANDLESERIALDATA_H

#include <QObject>
#include <QString>

//数据包格式：0xAA 0x55 length type data
enum myStatesType
{
    NEEDAA = 100,
    NEED55,
    NEEDLEN
};


enum myDataType
{
    myDataType_Quat = 0,
    myDataType_Accel,
    myDataType_Gyro,
    myDataType_Pause,
    myDataType_Run,
    myDataType_Restart
};


class handleserialdata : public QObject
{
    Q_OBJECT
public:
    explicit handleserialdata(QObject *parent = 0);

    void handleTheBuf(const QByteArray &bytes);
    void handleOneByte(const uint8_t &byte);
    void frameCompleted(QByteArray &myBuf,int &mylength);
    void handleQuatData(long quat[]);
    void handleAccelData(short *accel);
    void handleGyroData(short *gyro);
    float math_rsqrt(float number);

private:
    int myStates,myLength;
    QByteArray myBuf;
signals:
    void sendHandledData(float &w,float &x,float &y,float &z,float &pitch,float &roll,float &yaw);
public slots:
};

#endif // HANDLESERIALDATA_H
