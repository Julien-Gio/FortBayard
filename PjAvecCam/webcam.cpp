#include "webcam.h"

Webcam::Webcam()
{

    webCam_=new cv::VideoCapture(0);
    int width=webCam_->get(cv::CAP_PROP_FRAME_WIDTH);
    int height=webCam_->get(cv::CAP_PROP_FRAME_HEIGHT);

    th = new std::thread(&Webcam::updateCamera, this);
}

Webcam::~Webcam(){
    delete th;
    delete webCam_;
}

void Webcam::updateCamera(){
    while(true){
        if (webCam_->isOpened()) {
            Mat image;
        }
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}
