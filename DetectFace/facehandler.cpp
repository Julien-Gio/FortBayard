#include "facehandler.h"

FaceHandler::FaceHandler() :
    cap(0) {
    cout << "===================================" << endl;
    cout << "          START OF PROGRAM         " << endl;
    cout << "===================================" << endl;
    cout << "D to toggle debugging graphics." << endl;
    cout << "Escape to quit." << endl << endl;

    state = 'N';


    cout << "Frame width :" << cap.get(CAP_PROP_FRAME_WIDTH) << endl;
    cout << "Frame height :" << cap.get(CAP_PROP_FRAME_HEIGHT) << endl;
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

    // Init output window
    namedWindow("WebCam", 1);
}


void FaceHandler::update() {
    // Get frame2
    cap >> frame;
    // Mirror effect
    cv::flip(frame, frame, 1);

    // Detect Faces //
    // Convert to gray
    Mat frame_gray;
    cv::cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
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
            Mat faceMat;
            cv::cvtColor(Mat(frame, average_face), faceMat, COLOR_BGR2GRAY);
            templateRect.width = (average_face.width * templateWidth);
            templateRect.height = (average_face.height * templateHeight);
            templateRect.x = average_face.width / 2 - templateRect.width / 2 + (average_face.width * templateOffsetX);
            templateRect.y = average_face.height / 2 - templateRect.height / 2 + (average_face.height * templateOffsetY);
            templateImage = Mat(faceMat, templateRect);

            // 2) Passer en mode continue
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
    imshow("WebCam", frame);
}


void FaceHandler::update_C(Rect& face) {
    // Pour toujours avoir un nombre constant de faces, enlever une face puis la remplacer avec la plus récente
    faces.erase(faces.begin());
    faces.emplace_back(face);

    Rect average_face = getAverageFace();

    // Detect Motion //
    // Extract working rect in frame2 and convert to gray
    cv::cvtColor(Mat(frame, average_face), frameRect, COLOR_BGR2GRAY);

    // resultImage stocke la resultat de la Xcor entre l et le tamplate.
    // Sa taille est donc cette d'une frame moins la moitier du template en haut, en bas, à gauche et à droite
    int result_cols = frameRect.cols - templateImage.cols + 1;
    int result_rows = frameRect.rows - templateImage.rows + 1;
    resultImage.create(result_cols, result_rows, CV_32FC1);

    // Do the Matching between the face and the template
    matchTemplate(frameRect, templateImage, resultImage, TM_CCORR_NORMED);

    // Localize the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc);

    // Compute the translation vector between the origin and the matching rect
    Point vect(maxLoc.x - templateRect.x, maxLoc.y - templateRect.y);

    // Get the direction
    Point dir(0, 0);
    if (vect.x >= SEUIL)
        dir.x = 1;
    else if (vect.x <= -SEUIL)
        dir.x = -1;
    if (vect.y >= SEUIL)
        dir.y = 1;
    else if (vect.y <= -SEUIL)
        dir.y = -1;

    // Draw the direction
    Point dirCenter(frameWidth / 2, frameHeight - 40);
    Point dirEdge(dirCenter.x + dir.x * 45, dirCenter.y + dir.y * 35);
    arrowedLine(frame, dirCenter, dirEdge, Scalar(0, 0, 255), 5);

    if (debug_graphics) {
        // GREEN RECT : face
        rectangle(frame, average_face, Scalar(0, 255, 0), 2);

        // Draw the translation vector
        Point faceCenter(average_face.x + average_face.width / 2, average_face.y + average_face.height / 2);
        Point p(faceCenter.x + vect.x, faceCenter.y + vect.y);
        arrowedLine(frame, faceCenter, p, Scalar(255, 255, 255), 2);

        // BLACK RECT : template rect
        Rect blackSquare(average_face.x + templateRect.x, average_face.y + templateRect.y,
                         templateRect.width, templateRect.height);
        rectangle(frame, blackSquare, Scalar(0, 0, 0), 2);

        // PURPLE DOT : center of result
        rectangle(frame, Rect(average_face.x + templateRect.width / 2 + maxLoc.x,
                               average_face.y + templateRect.height / 2 + maxLoc.y, 5, 5),
                  Scalar(230, 0, 230), 3);
    }

}


void FaceHandler::update_J(Rect& face) {
    // Ajouter la face à notre buffer
    faces.emplace_back(face);

    Rect average_face = getAverageFace();

    if (debug_graphics) {
        // Draw yellow rectangle
        rectangle(frame, average_face, Scalar(0, 200, 220), 2);
    }
}


void FaceHandler::update_N(Rect& face) {
    // Draw red rectangles
    rectangle(frame, Rect(20, 20, 15, 15), Scalar(0, 0, 250), 4);
    rectangle(frame, Rect(45, 20, 15, 15), Scalar(0, 0, 250), 3);
    rectangle(frame, Rect(70, 20, 15, 15), Scalar(0, 0, 250), 2);
}


Rect FaceHandler::getFace(Mat* frame_gray) {
    std::vector<Rect> faces;

    std::vector<int> rejectLevels;
    std::vector<double> levelWeights;

    face_cascade.detectMultiScale(*frame_gray, faces, rejectLevels, levelWeights, 1.1, 3, 0, Size(60, 60), Size(), true);

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

