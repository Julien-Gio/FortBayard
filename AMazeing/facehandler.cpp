#include "facehandler.h"
#include "mainwindow.h"

FaceHandler::FaceHandler(Maze* maze_) :
    cap(0) {
//    std::cout << "===================================" << std::endl;
//    std::cout << "          START OF PROGRAM         " << std::endl;
//    std::cout << "===================================" << std::endl;
//    std::cout << "D to toggle debugging graphics." << std::endl;
//    std::cout << "Escape to quit." << std::endl << std::endl;
//    std::cout << "Frame width :" << cap.get(CAP_PROP_FRAME_WIDTH) << std::endl;
//    std::cout << "Frame height :" << cap.get(CAP_PROP_FRAME_HEIGHT) << std::endl;

    cap.set(CAP_PROP_FRAME_WIDTH, frameWidth);
    cap.set(CAP_PROP_FRAME_HEIGHT, frameHeight);
    if(!cap.isOpened())  // check if we succeeded
    {
        std::cerr << "Error openning the default camera!" << std::endl;
        return;
    }

    // Detection de faces
    if(!faceCascade.load("../DetectFace/haarcascade_frontalface_alt.xml")) {
        std::cerr << "Error loading haarcascade!" << std::endl;
        return;
    }

    state = 'N';
    maze = maze_;

    // Init output window
    //namedWindow("WebCam", 1);

    // Create the video tread
    th = new std::thread(&FaceHandler::update, this);
}


FaceHandler::~FaceHandler() {
    delete th;
}


void FaceHandler::update() {
    while (true) {
        // Get frame
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
                // Pas de visage => passage en mode N
                state = 'N';
            } else {
                updateC(face);
            }
            break;
        case 'J':
            if (face.width <= 0) {
                // Pas de visage => passage en mode N
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
                updateJ(face);
            }
            break;
        case 'N':
            if (face.width > 0) {
                faces.clear();
                state = 'J';
            } else {
                updateN(face);
            }
            break;
        }

        // Display frame
        //imshow("WebCam", frame);
        QImage img = QImage((const unsigned char*)(frame.data), frame.cols, frame.rows, QImage::Format_BGR888);

        if (mw != NULL) {
            mw->setImageLbl(img);
        }


        this_thread::sleep_for(chrono::milliseconds(50));
    }
}


void FaceHandler::updateC(Rect& face) {
    // Pour toujours avoir un nombre constant de faces, enlever une face puis la remplacer avec la plus récente
    faces.erase(faces.begin());
    faces.emplace_back(face);

    // Récupérer la moyenne des X deniers visages pour que les mouvements de tête soient plus fluides
    Rect averageFace = getAverageFace();

    // Detect Motion //
    // Extract face in frame and convert to gray
    cv::cvtColor(Mat(frame, averageFace), frameRect, COLOR_BGR2GRAY);

    // resultImage stocke la resultat de la Xcor entre le visage et le tamplate
    // La corrélation ignore les bords
    // Sa taille est donc celle du visage moins la moitier du template en haut, en bas, à gauche et à droite
    int result_cols = frameRect.cols - templateImage.cols + 1;
    int result_rows = frameRect.rows - templateImage.rows + 1;
    resultImage.create(result_cols, result_rows, CV_32FC1);

    // Do the Matching between the face and the template
    matchTemplate(frameRect, templateImage, resultImage, TM_CCORR_NORMED);

    // Localize the best match with minMaxLoc
    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc);

    // Compute the translation vector between the origin and the matching rect
    cv::Point vect(maxLoc.x - templateRect.x, maxLoc.y - templateRect.y);

    // Get the direction
    cv::Point dir(0, 0);
    if (vect.x >= SEUILS[DROITE]) {  // Droite
        dir.x = 1;
        maze->rotate(ROTATION_SPEED * vect.x);
    } else if (vect.x <= -SEUILS[GAUCHE]) { // Gauche
        dir.x = -1;
        maze->rotate(ROTATION_SPEED * vect.x);
    }
    if (vect.y >= SEUILS[BAS]) { // Bas
        dir.y = 1;
        maze->idle();
    } else if (vect.y <= -SEUILS[HAUT]) { // Haut
        dir.y = -1;
        maze->walk(WALK_SPEED);
    }

    if (dir.x == 0 && dir.y != -1) {
        maze->idle();
    }

    // Draw the direction
    cv::Point dirCenter(frameWidth / 2, frameHeight - 40);
    cv::Point dirEdge(dirCenter.x + dir.x * 45, dirCenter.y + dir.y * 35);
    arrowedLine(frame, dirCenter, dirEdge, Scalar(0, 0, 255), 5);

    if (debugGraphics) {
        // GREEN RECT : face
        rectangle(frame, averageFace, Scalar(0, 255, 0), 2);

        // Draw the translation vector
        cv::Point faceCenter(averageFace.x + averageFace.width / 2, averageFace.y + averageFace.height / 2);
        cv::Point p(faceCenter.x + vect.x, faceCenter.y + vect.y);
        arrowedLine(frame, faceCenter, p, Scalar(255, 255, 255), 2);

        // BLACK RECT : template rect
        Rect blackSquare(averageFace.x + templateRect.x, averageFace.y + templateRect.y,
                         templateRect.width, templateRect.height);
        rectangle(frame, blackSquare, Scalar(0, 0, 0), 2);

        // PURPLE DOT : center of result
        rectangle(frame, Rect(averageFace.x + templateRect.width / 2 + maxLoc.x,
                               averageFace.y + templateRect.height / 2 + maxLoc.y, 5, 5),
                  Scalar(230, 0, 230), 3);
    }

}


void FaceHandler::updateJ(Rect& face) {
    // Ajouter la face à notre buffer
    faces.emplace_back(face);

    Rect averageFace = getAverageFace();

    if (debugGraphics) {
        // Draw yellow rectangle around face
        rectangle(frame, averageFace, Scalar(0, 200, 220), 2);
    }
}


void FaceHandler::updateN(Rect& face) {
    // Draw red rectangles
    rectangle(frame, Rect(20, 20, 15, 15), Scalar(0, 0, 250), 4);
    rectangle(frame, Rect(45, 20, 15, 15), Scalar(0, 0, 250), 3);
    rectangle(frame, Rect(70, 20, 15, 15), Scalar(0, 0, 250), 2);
}


Rect FaceHandler::getFace(Mat* frame_gray) {
    std::vector<Rect> faces;

    std::vector<int> rejectLevels;
    std::vector<double> levelWeights;

    faceCascade.detectMultiScale(*frame_gray, faces, rejectLevels, levelWeights, 1.1, 3, 0, Size(60, 60), Size(), true);

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

