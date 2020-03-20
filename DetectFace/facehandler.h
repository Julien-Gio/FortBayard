#ifndef FACEHANDLER_H
#define FACEHANDLER_H

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/objdetect.hpp>

#include <cstdio>
#include <iostream>


using namespace cv;
using namespace std;


class FaceHandler {
    const int frameWidth = 320;
    const int frameHeight = 240;
    const int subImageWidth = 100;
    const int subImageHeight = 100;
    const float templateWidth = 0.4;  // Percentage of face
    const float templateHeight = 0.3;  // Percentage of face
    const float templateOffsetX = 0.0;  // Percentage of face. < 0 to the left, > 0 to the right
    const float templateOffsetY = 0.2;  // Percentage of face. < 0 above center, > 0 below center

    const float SEUIL = 2.0;

    bool debug_graphics = true;

    const unsigned int NUM_AVG_OVER_FRAMES = 10;  // Nombre de frames du visage en mémoire pour trouver la moyenne
    /* STATES
     * 'C' : continously detecting faces
     * 'N' : currently no face is detected
     * 'J' : just detected a face
     *
     * C / J => n if no face is detected
     * N => J if a face is detected
     * J => C if faces are detected for long enough
     *
     * movement calculations are only done in state 'C'
    **/
    const char STATES[3] = {'C', 'J', 'N'};
    char state;

    std::vector<Rect> faces;
    VideoCapture cap;
    CascadeClassifier face_cascade;

    Rect templateRect;
    Mat templateImage;  // to store the nose
    Mat resultImage;    // to store the matchTemplate result

    Mat frame, frameRect;

public:
    FaceHandler();

    void update();
    void toogle_debug_graphics() {
        debug_graphics = !debug_graphics;
    }

private:
    void update_C(Rect& face);
    void update_J(Rect& face);
    void update_N(Rect& face);
    Rect getFace(Mat* frame_gray);
    Rect getAverageFace();

};

#endif // FACEHANDLER_H
