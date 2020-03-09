#ifndef WEBCAM_H
#define WEBCAM_H

#include "opencv2/opencv.hpp"
#include <thread>
#include <QWidget>
#include <QLabel>
#include <iostream>
using namespace std;

using namespace cv;

class Webcam
{
    QLabel* infoLabel, *imageLabel;
    std::thread* th;
    cv::VideoCapture * webCam_;

    void updateCamera();
public:
    Webcam(QLabel*, QLabel*);
    ~Webcam();
};

#endif // WEBCAM_H
