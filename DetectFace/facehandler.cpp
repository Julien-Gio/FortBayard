#include "facehandler.h"

FaceHandler::FaceHandler() :
    cap(0),
    workingRect((frameWidth-subImageWidth)/2, frameHeight/2+(frameHeight/2-subImageHeight)/2, subImageWidth, subImageHeight),
    templateRect((workingRect.width-templateWidth)/2, (workingRect.height-templateHeight)/2, templateWidth, templateHeight),
    workingCenter(workingRect.x+subImageWidth/2, workingRect.y+subImageHeight/2) {

    cout << "===================================" << endl;
    cout << "          START OF PROGRAM         " << endl;
    cout << "===================================" << endl;
    cout << "D to toggle debugging graphics." << endl;
    cout << "Escape to quit." << endl;

    state = 'N';


    cout << "width :" << cap.get(CAP_PROP_FRAME_WIDTH) << endl;
    cout << "height :" << cap.get(CAP_PROP_FRAME_HEIGHT) << endl;
    cap.set(CAP_PROP_FRAME_WIDTH,frameWidth);
    cap.set(CAP_PROP_FRAME_HEIGHT,frameHeight);
    if(!cap.isOpened())  // check if we succeeded
    {
        cerr<<"Error openning the default camera"<<endl;
        return;
    }

    // Detection de faces
    if(!face_cascade.load("../DetectFace/haarcascade_frontalface_alt.xml")) {
        cerr << "Error loading haarcascade"<< endl;
        return;
    }

    cout << "FaceHandler created." << endl;



    // Init output window
    namedWindow("WebCam", 1);
}


void FaceHandler::update() {
    // Get frame2
    cap >> frame2;
    // Mirror effect
    cv::flip(frame2, frame2, 1);

    // Detect Faces //
    Mat frame_gray;
    // Convert to gray
    cv::cvtColor(frame2, frame_gray, COLOR_BGR2GRAY);
    Rect face = getFace(&frame_gray);


    switch (state) {
    case 'C':
        if (face.width <= 0) {
            // Pas de visage
            state = 'N';
        } else {
            update_C(face);
        }
        break;
    case 'J':
        if (face.width <= 0) {
            // Pas de visage
            state = 'N';
        } else if (faces.size() == NUM_AVG_OVER_FRAMES) {
            // Nous avons stabilisé
            // 1) Récupérer l'image de référence
            Rect average_face = getAverageFace();
            cv::cvtColor(Mat(frame2, average_face), frameRect1, COLOR_BGR2GRAY);
            templateRect.width = (average_face.width * templateWidth);
            templateRect.height = (average_face.height * templateHeight);
            templateRect.x = average_face.width / 2 - templateRect.width / 2 + (average_face.width * templateOffsetX);
            templateRect.y = average_face.height / 2 - templateRect.height / 2 + (average_face.height * templateOffsetY);
            templateImage = Mat(frameRect1, templateRect);

            // 2) Reset mouvement
            forward_mvmt = 0;
            side_mvmt = 0;

            // 3) Passer en mode continue
            state = 'C';
        } else {
            update_J(face);
        }
        break;
    case 'N':
        if (face.width > 0) {
            faces.clear();
            state = 'J';
        } else {
            update_N(face);
        }
        break;
    }

    // Display frame2
    imshow("WebCam", frame2);
}


void FaceHandler::update_C(Rect& face) {
    // Pour toujours avec une nombre constant de faces, enlever une face puis la remplacer avec la plus récente
    faces.emplace_back(face);
    faces.erase(faces.begin());

    Rect average_face = getAverageFace();

    // Detect Motion //
    // Extract working rect in frame2 and convert to gray
    cv::cvtColor(Mat(frame2, average_face), frameRect2, COLOR_BGR2GRAY);

    // Extract template image in frame1
    //Mat templateImage(frameRect1, templateRect);

    // resultImage stocke la resultat de la Xcor entre l et le tamplate.
    // Sa taille est donc cette d'une frame moins la moitier du template en haut, en bas, à gauche et à droite
    int result_cols = frameRect2.cols - templateImage.cols + 1;
    int result_rows = frameRect2.rows - templateImage.rows + 1;
    resultImage.create(result_cols, result_rows, CV_32FC1);
    cout << "corr:" << resultImage.rows << " " << resultImage.cols << endl;
    cout << "img:" << frameRect2.rows << " " << frameRect2.cols << endl;
    cout << "templ:" << templateImage.rows << " " << templateImage.cols << endl;
    // Do the Matching between the working rect in frame2 and the templateImage in frame1
    matchTemplate(frameRect2, templateImage, resultImage, TM_CCORR_NORMED);

    // Localize the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc);

    // Compute the translation vector between the origin and the matching rect
    Point vect(maxLoc.x-templateRect.x, maxLoc.y-templateRect.y);
    // Draw the direction
    if (abs(vect.y) >= SEUIL)
        forward_mvmt += vect.y;
    if (abs(vect.x) >= SEUIL)
        side_mvmt += vect.x;

    // Green rect around face
    rectangle(frame2, average_face, Scalar(0, 255, 0), 2);

    if (debug_graphics) {
        // Draw the translation vector
        Point faceCenter(average_face.x + average_face.width / 2, average_face.y + average_face.height / 2);
        Point p(faceCenter.x+vect.x, faceCenter.y+vect.y);
        arrowedLine(frame2, faceCenter, p, Scalar(255, 255, 255), 2);

        // Draw template in face
        //cout << maxLoc.x << ", " << maxLoc.y << endl;
        rectangle(frame2, Rect(average_face.x + maxLoc.x, average_face.y + maxLoc.y, 8, 8), Scalar(200, 0, 200), 3);
        Rect blackSquare(average_face.x + templateRect.x, average_face.y + templateRect.y, templateRect.width, templateRect.height);
        rectangle(frame2, blackSquare, Scalar(0, 0, 0), 2);
        rectangle(frame2, workingRect, Scalar(230, 40, 40), 2);

        //std::cout << vect.x << " " << vect.y << std::endl;
        Point dirCenter(frameWidth / 2, frameHeight - 20);
        Point dirEdge(dirCenter.x + side_mvmt, dirCenter.y + forward_mvmt);
        arrowedLine(frame2, dirCenter, dirEdge, Scalar(0, 0, 255), 3);
    }

    // Swap matrixes
    //swap(frameRect1, frameRect2);
}


void FaceHandler::update_J(Rect& face) {
    // Ajouter la face à notre buffer
    faces.emplace_back(face);

    Rect average_face = getAverageFace();

    // Draw yellow rectangle
    rectangle(frame2, average_face, Scalar(0, 200, 220), 2);
}


void FaceHandler::update_N(Rect& face) {
    // Draw red rectangles
    rectangle(frame2, Rect(10, 10, 10, 10), Scalar(0, 0, 250), 2);
    rectangle(frame2, Rect(25, 10, 10, 10), Scalar(0, 0, 250), 2);
    rectangle(frame2, Rect(40, 10, 10, 10), Scalar(0, 0, 250), 2);
}


Rect FaceHandler::getFace(Mat* frame_gray) {
    std::vector<Rect> faces;

    std::vector<int> rejectLevels;
    std::vector<double> levelWeights;
cout << "ee" << endl;
    face_cascade.detectMultiScale(*frame_gray, faces, rejectLevels, levelWeights, 1.1, 3, 0, Size(60, 60), Size(), true); //, 0, Size(60, 60), Size(300, 300));
cout << "oo" << endl;
    if (faces.size() > 0) {
        return faces[0];
    } else {
        return Rect(0, 0, 0, 0);
    }
}


Rect FaceHandler::getAverageFace() {
    Rect sum(0, 0, 0, 0);
    for (Rect f : faces) {
        sum.x += f.x;
        sum.y += f.y;
        sum.width += f.width;
        sum.height += f.height;
    }
    sum.x /= (float)faces.size();
    sum.y /= (float)faces.size();
    sum.width /= (float)faces.size();
    sum.height /= (float)faces.size();

    return sum;
}

