#include "webcam.h"

Webcam::Webcam(QLabel* infoL, QLabel* imageL)
{
    infoLabel = infoL;
    imageLabel = imageL;

    webCam_=new cv::VideoCapture(0);
    int width=webCam_->get(cv::CAP_PROP_FRAME_WIDTH);
    int height=webCam_->get(cv::CAP_PROP_FRAME_HEIGHT);

    if(!webCam_->isOpened())  // check if we succeeded
    {
        infoLabel->setText("Error openning the default camera !");
    }
    else
    {
        infoLabel->setText(QString("Video ok, image size is %1x%2 pixels").arg(width).arg(height));
    }

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
            if (webCam_->read(image)) {   // Capture a frame
                // Flip to get a mirror effect
                flip(image,image,1);
                // Invert Blue and Red color channels
                cvtColor(image,image,COLOR_BGR2RGB);
                // Convert to Qt image
                QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
                // Display on label
                imageLabel->setPixmap(QPixmap::fromImage(img));
                // Resize the label to fit the image
                imageLabel->resize(imageLabel->pixmap()->size());

            }
            else {
               infoLabel->setText("Error capturing the frame");
            }
        }
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}
