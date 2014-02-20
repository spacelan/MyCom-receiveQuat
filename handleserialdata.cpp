//     Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.

#include "handleserialdata.h"
#include <math.h>

handleserialdata::handleserialdata(QObject *parent) :
    QObject(parent)
{
    myStates = NEEDAA;
    myLength = 0;
    myBuf.clear();
}

void handleserialdata::handleTheBuf(const QByteArray &bytes)
{
    int len = bytes.size();
    int i;
    for(i=0;i<len;i++)
        handleOneByte((uint8_t) bytes[i]);
}

void handleserialdata::handleOneByte(const uint8_t &byte)
{
    if(myStates == NEEDAA)
    {
        if(byte == 0xaa)
            myStates = NEED55;
        return;
    }
    if(myStates == NEED55)
    {
        if(byte == 0x55)
            myStates = NEEDLEN;
        return;
    }
    if(myStates == NEEDLEN)
    {
        myLength = byte;
        myStates = 0;
        myBuf.clear();
        return;
    }
    if(myStates < NEEDAA)
    {
        myBuf[myStates] = byte;
        myStates++;
        if(myStates == myLength)
        {
            frameCompleted(myBuf,myLength);
            myStates = NEEDAA;
        }
        return;
    }
}

void handleserialdata::frameCompleted(QByteArray &myBuf, int &mylength)
{
    int type = myBuf[0];
    myBuf.remove(0,1);

    switch(type)
    {
    case myDataType_Quat:
    {
        if(mylength - 1 != 16) return;
        long quat[4]={0,0,0,0};
        quat[0] |= (myBuf[0] << 24) & 0xff000000;
        quat[0] |= (myBuf[1] << 16) & 0x00ff0000;
        quat[0] |= (myBuf[2] <<  8) & 0x0000ff00;
        quat[0] |= (myBuf[3] <<  0) & 0x000000ff;
        quat[1] |= (myBuf[4] << 24) & 0xff000000;
        quat[1] |= (myBuf[5] << 16) & 0x00ff0000;
        quat[1] |= (myBuf[6] <<  8) & 0x0000ff00;
        quat[1] |= (myBuf[7] <<  0) & 0x000000ff;
        quat[2] |= (myBuf[8] << 24) & 0xff000000;
        quat[2] |= (myBuf[9] << 16) & 0x00ff0000;
        quat[2] |= (myBuf[10]<<  8) & 0x0000ff00;
        quat[2] |= (myBuf[11]<<  0) & 0x000000ff;
        quat[3] |= (myBuf[12]<< 24) & 0xff000000;
        quat[3] |= (myBuf[13]<< 16) & 0x00ff0000;
        quat[3] |= (myBuf[14]<<  8) & 0x0000ff00;
        quat[3] |= (myBuf[15]<<  0) & 0x000000ff;
        handleQuatData(quat);
    }break;
    case myDataType_Accel:
    {
        if(mylength - 1 != 6) return;
        short accel[3] = {0,0,0};
        accel[0] |= (myBuf[0] << 8) & 0xff00;
        accel[0] |= (myBuf[1] << 0) & 0x00ff;
        accel[1] |= (myBuf[2] << 8) & 0xff00;
        accel[1] |= (myBuf[3] << 0) & 0x00ff;
        accel[1] |= (myBuf[4] << 8) & 0xff00;
        accel[1] |= (myBuf[5] << 0) & 0x00ff;
        handleAccelData(accel);
    }break;
    case myDataType_Gyro:
    {
        if(mylength - 1 != 6) return;
        short gyro[3] = {0,0,0};
        gyro[0] |= (myBuf[0] << 8) & 0xff00;
        gyro[0] |= (myBuf[1] << 0) & 0x00ff;
        gyro[1] |= (myBuf[2] << 8) & 0xff00;
        gyro[1] |= (myBuf[3] << 0) & 0x00ff;
        gyro[1] |= (myBuf[4] << 8) & 0xff00;
        gyro[1] |= (myBuf[5] << 0) & 0x00ff;
        handleGyroData(gyro);
    }break;
    }

}

void handleserialdata::handleQuatData(long quat[4])
{
    float q30 = 1073741824.0f;
    float w,x,y,z;
    w = (float)quat[0] / q30;
    x = (float)quat[1] / q30;
    y = (float)quat[2] / q30;
    z = (float)quat[3] / q30;

    float norm = math_rsqrt(w*w + x*x + y*y + z*z);
    w *= norm;
    x *= norm;
    y *= norm;
    z *= norm;

    float pitch,roll,yaw;
    pitch = asin(2*w*y - 2*z*x) * 57.3;
    roll = atan2(2*w*x + 2*y*z,1 - 2*x*x - 2*y*y) * 57.3;
    yaw = atan2(2*w*z + 2*x*y,1 - 2*y*y - 2*z*z) * 57.3;

    emit sendHandledData(w,x,y,z,pitch,roll,yaw);
}

void handleserialdata::handleAccelData(short *accel)
{

}

void handleserialdata::handleGyroData(short *gyro)
{

}

float handleserialdata::math_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking（对浮点数的邪恶位级hack）
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?（这他妈的是怎么回事？）
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration （第一次牛顿迭代）
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed（第二次迭代，可以删除）

    return y;
}
