#include "opencv2/opencv.hpp"
#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <iostream>

using namespace cv;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    webcamThread = new Webcam(ui->infoLabel_, ui->imageLabel_);
}


Widget::~Widget()
{
    delete webcamThread;
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_D:
        ui->scrollAreaWidgetContents->setRotation(ui->scrollAreaWidgetContents->getRotation()+2 * 3.14159/180);
        if(!ui->scrollArea->isHidden())
            ui->scrollAreaWidgetContents->update();
        break;
    case Qt::Key_Q:
        ui->scrollAreaWidgetContents->setRotation(ui->scrollAreaWidgetContents->getRotation()-2 * 3.14159/180);
        if(!ui->scrollArea->isHidden())
            ui->scrollAreaWidgetContents->update();
        break;
    case Qt::Key_R:
        if(ui->scrollArea->isHidden()){
            ui->scrollAreaWidgetContents->update();
            ui->scrollArea->show();
        }
        else
            ui->scrollArea->hide();
        break;
    }
}
