#include "opencv2/opencv.hpp"
#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <iostream>

using namespace cv;

Widget::Widget(QWidget *parent) :
    QGLWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    webcamThread = new Webcam();
}


Widget::~Widget()
{
    delete webcamThread;
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event){
}
