//     Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.

#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include <QtGlobal>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    Q_INIT_RESOURCE(image);
    Widget w;
    w.show();
    
    return a.exec();
}
