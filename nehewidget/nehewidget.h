//     Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.

#ifndef NEHEWIDGET_H
#define NEHEWIDGET_H

#include <QWidget>
#include <qgl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QTimer>
#include <QImage>
#include <QCloseEvent>


namespace Ui {
class NeHeWidget;
}

class NeHeWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit NeHeWidget(QGLWidget *parent = 0,bool fs = false);
    ~NeHeWidget();

private:
    Ui::NeHeWidget *ui;
    float Matrix4[16];

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int hight);
    void keyPressEvent(QKeyEvent *e);
    void loadGLTextures();
    void closeEvent(QCloseEvent *e);

    bool fullscreen;
    GLfloat rTri;
    GLfloat rQuad;
    QTimer *myTimer;
    GLfloat xRot,yRot,zRot;
    GLuint texture[2];

public slots:
    void quatToMatrix(float w, float x, float y, float z);

signals:
    void closeWidget();

};

#endif // NEHEWIDGET_H
