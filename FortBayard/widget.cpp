#include "opencv2/opencv.hpp"
#include "widget.h"
#include "ui_widget.h"
#include <QString>

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

void Widget::on_captureButton__clicked()
{

}
