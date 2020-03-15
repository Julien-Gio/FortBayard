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
    std::thread* th; // Composition
    cv::VideoCapture * webCam_; // Composition

    void updateCamera();
public:
    Webcam();
    ~Webcam();
};

#endif // WEBCAM_H
