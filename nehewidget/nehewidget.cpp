//     Copyright (c) 2014 spacelan1993@gmail.com All rights reserved.

#include "nehewidget.h"
#include "ui_nehewidget.h"
#include "math.h"

GLfloat light_ambient[4]={0.5, 0.5, 0.5, 1.0};
GLfloat light_diffuse[4]={1.0, 1.0, 1.0, 1.0};
GLfloat light_position[4]={0.0, 0.0, 2.0, 0.0};
bool light = true;

//构造函数
NeHeWidget::NeHeWidget(QGLWidget *parent, bool fs) :
    QGLWidget(parent),
    ui(new Ui::NeHeWidget)
{
    ui->setupUi(this);

    quatToMatrix(1,0,0,0);

    //旋转角度
    rTri = 0;
    rQuad = 0;

    xRot = yRot = zRot = 0;

    //定时更新
    myTimer = new QTimer(this);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(update()));
    myTimer->setInterval(10);
    myTimer->start();

    //窗口位置标题
    setGeometry(100,100,648,480);
    setWindowTitle(tr("终于弄好了有木有！！！"));

    //是否全屏
    fullscreen = fs;
    if(fullscreen)
        showFullScreen();
}

//析构函数
NeHeWidget::~NeHeWidget()
{
    delete ui;
}

//初始化函数
void NeHeWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0,0.0,0.0,0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    //纹理
    loadGLTextures();
    glEnable( GL_TEXTURE_2D );

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);//指定光源1的环境光参数
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);//指定光源1的漫射光参数
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);//指定光源1的位置
    glEnable(GL_LIGHT1);//允许光源1的使用
//    glEnable(GL_LIGHTING);//我们还需要启动总光源开关,默认的时候不开，后面的L键来控制开启和关闭
    glColor4f(1.0, 1.0, 1.0, 0.5);//后面的步骤都是以全亮绘制物体，并且50%的透明度
}

//绘制函数
void NeHeWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//    //绘制金字塔
//    glLoadIdentity();
//    glTranslatef(-1.5,0.0,-6.0);
//    glRotatef(rTri,1,0,0);
//    glBegin(GL_TRIANGLES);
//        //正面
//        glColor3f(1,0,0);
//        glVertex3f(0,1,0);
//        glColor3f(0,1,0);
//        glVertex3f(-1,-1,1);
//        glColor3f(0,0,1);
//        glVertex3f(1,-1,1);
//        //右侧面
//        glColor3f(1,0,0);
//        glVertex3f(0,1,0);
//        glColor3f(0,0,1);
//        glVertex3f(1,-1,1);
//        glColor3f(0,1,0);
//        glVertex3f(1,-1,-1);
//        //后面
//        glColor3f(1,0,0);
//        glVertex3f(0,1,0);
//        glColor3f(0,1,0);
//        glVertex3f(1,-1,-1);
//        glColor3f(0,0,1);
//        glVertex3f(-1,-1,-1);
//        //左侧面
//        glColor3f(1,0,0);
//        glVertex3f(0,1,0);
//        glColor3f(0,0,1);
//        glVertex3f(-1,-1,-1);
//        glColor3f(0,1,0);
//        glVertex3f(-1,-1,1);
//    glEnd();
//    //底面
//    glBegin(GL_QUADS);
//        glColor3f(0,1,0);
//        glVertex3f(-1,-1,1);
//        glColor3f(0,0,1);
//        glVertex3f(-1,-1,-1);
//        glColor3f(0,1,0);
//        glVertex3f(1,-1,-1);
//        glColor3f(0,0,1);
//        glVertex3f(1,-1,1);
//    glEnd();

    //绘制正方体
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    //真实视点
    gluLookAt(0,-6,2,0,0,0,0,0,1);

//    glTranslatef(0,0,-6);
//    quatToMatrix(0.928884f, 0.370318f, -0.005363f, 0.001255f);
    glMultMatrixf(Matrix4);
//    glRotatef(rQuad,1,2,3);
    glRotatef(xRot,1,0,0);
    glRotatef(yRot,0,1,0);
    glRotatef(zRot,0,0,1);
    glBindTexture( GL_TEXTURE_2D,texture);
    glBegin(GL_QUADS);
        //z+
        glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0,  0.1 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0,  0.1 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  0.1 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0,  0.1 );
        //z-
        glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0, -0.1 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0, -0.1 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0, -0.1 );
        glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0, -0.1 );
        //y+
        glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -0.1 );
        glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0,  1.0,  0.1 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  0.1 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -0.1 );
        //y-
        glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0, -1.0, -0.1 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0, -1.0, -0.1 );
        glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  0.1 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  0.1 );
        //x+
        glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0, -0.1 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -0.1 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0,  0.1 );
        glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  0.1 );
        //x-
        glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, -0.1 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  0.1 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0,  0.1 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -0.1 );
    glEnd();

    //旋转
    rTri += 0.2;
    rQuad -= 0.1;
//    xRot += 0.3;
//    yRot += 0.2;
//    zRot += 0.4;
    glPopMatrix();
}


void NeHeWidget::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0,0,(GLint) width,(GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(GLfloat) width / (GLfloat) height,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//键盘事件
void NeHeWidget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_F2:
        fullscreen = !fullscreen;
        if(fullscreen)
            showFullScreen();
        else
        {
            showNormal();
           // setGeometry();
        }
        updateGL();
        break;
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Up:
        xRot -= 1;
        break;
    case Qt::Key_Down:
        xRot += 1;
        break;
    case Qt::Key_Left:
        yRot -= 1;
        break;
    case Qt::Key_Right:
        yRot += 1;
        break;
    case Qt::Key_L:
        light = !light;
        if(!light)
            glDisable(GL_LIGHTING);
        else
            glEnable(GL_LIGHTING);
        updateGL();
        break;
    default:
        break;
    }
    updateGL();
}

void NeHeWidget::loadGLTextures()
{
    QImage tex,buf;
    if(!buf.load("../MyCom-receiveQuat/nehewidget/ican.jpg"))
    {
        qWarning("Could not read image file, using single-color instead!");
        QImage dummy(128,128,QImage::Format_RGB32);
        dummy.fill(Qt::red);
        buf = dummy;
    }
    tex = QGLWidget::convertToGLFormat(buf);
    glGenTextures(1,&texture);
    glBindTexture( GL_TEXTURE_2D,texture );
    glTexImage2D( GL_TEXTURE_2D,0,3,tex.width(),tex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex.bits() );
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
}

void NeHeWidget::closeEvent(QCloseEvent *e)
{
    emit closeWidget();
    e->accept();
}

void NeHeWidget::quatToMatrix(float w, float x, float y, float z)
{
    //不知道为什么，旋转方向是反的，所以求反
    x = -x; y = -y; z = -z;

    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    // This calculation would be a lot more complicated for non-unit length quaternions
    // Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
    //   OpenGL
//    Matrix4 = {1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
//            2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
//            2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
//            0.0f, 0.0f, 0.0f, 1.0f};
    Matrix4[0] = 1.0f - 2.0f * (y2 + z2);
    Matrix4[1] = 2.0f * (xy - wz);
    Matrix4[2] = 2.0f * (xz + wy);
    Matrix4[3] = 0.0f;
    Matrix4[4] = 2.0f * (xy + wz);
    Matrix4[5] = 1.0f - 2.0f * (x2 + z2);
    Matrix4[6] = 2.0f * (yz - wx);
    Matrix4[7] = 0.0f;
    Matrix4[8] = 2.0f * (xz - wy);
    Matrix4[9] = 2.0f * (yz + wx);
    Matrix4[10] = 1.0f - 2.0f * (x2 + y2);
    Matrix4[11] = 0.0f;
    Matrix4[12] = 0.0f;
    Matrix4[13] = 0.0f;
    Matrix4[14] = 0.0f;
    Matrix4[15] = 1.0f;
}



