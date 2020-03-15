#ifndef WIDGET_H
#define WIDGET_H

#include "opencv2/opencv.hpp"
#include <QWidget>
#include <iostream>
#include <mutex>
#include "webcam.h"
#include <QGLWidget>

using namespace std;

using namespace cv;

namespace Ui {
class Widget;
}

class Widget : public QGLWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Widget *ui;
    Webcam* webcamThread;

private slots:
};

#endif // WIDGET_H
