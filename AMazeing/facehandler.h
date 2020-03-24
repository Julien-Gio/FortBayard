#ifndef FACEHANDLER_H
#define FACEHANDLER_H

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/objdetect.hpp>

#include <thread>
#include <cstdio>
#include <iostream>

#include "maze.h"


using namespace cv;


class FaceHandler {
    std::thread* th;
    Maze* maze;  // Passed in constructor. FaceHandler does not create nor delete maze

    const int HAUT = 0;
    const int DROITE = 1;
    const int BAS = 2;
    const int GAUCHE = 3;

    const float ROTATION_SPEED = 0.01;
    const float WALK_SPEED = 0.05;

    const int frameWidth = 320;
    const int frameHeight = 240;
    const int subImageWidth = 100;
    const int subImageHeight = 100;
    const float templateWidth = 0.4;  // Percentage of face
    const float templateHeight = 0.5;  // Percentage of face
    const float templateOffsetX = 0.0;  // Percentage of face. < 0 to the left, > 0 to the right
    const float templateOffsetY = 0.05;  // Percentage of face. < 0 above center, > 0 below center


    const float SEUILS[4] = {3.0, 2.0, 2.0, 2.0};  // Haut, droite, bas, gauche
    const unsigned int NUM_AVG_OVER_FRAMES = 12;  // Nombre de frames du visage en mémoire pour trouver la moyenne

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
    CascadeClassifier faceCascade;

    Rect templateRect;
    Mat templateImage;  // to store the nose
    Mat resultImage;    // to store the matchTemplate result

    Mat frame, frameRect;

    bool debugGraphics = true;

public:
    FaceHandler(Maze* maze_);
    ~FaceHandler();

    void update();

    void toogle_debug_graphics() {
        debugGraphics = !debugGraphics;
    }

private:
    void updateC(Rect& face);  // State 'C'
    void updateJ(Rect& face);  // State 'J'
    void updateN(Rect& face);  // State 'N'
    Rect getFace(Mat* frame_gray);
    Rect getAverageFace();

};

#endif // FACEHANDLER_H
