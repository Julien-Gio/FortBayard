#include "facehandler.h"

FaceHandler::FaceHandler() :
    cap(0),
    workingRect((frameWidth-subImageWidth)/2, frameHeight/2+(frameHeight/2-subImageHeight)/2, subImageWidth, subImageHeight),
    templateRect((workingRect.width-templateWidth)/2, (workingRect.height-templateHeight)/2, templateWidth, templateHeight),
    workingCenter(workingRect.x+subImageWidth/2, workingRect.y+subImageHeight/2) {

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
}


void FaceHandler::update() {
    Mat frame1,frame2,frameRect1,frameRect2;
    // Get frame2
    cap >> frame2;
    // Mirror effect
    cv::flip(frame2, frame2, 1);

    // Detect Faces //
    Mat frame_gray;
    // Convert to gray
    cv::cvtColor(frame2, frame_gray, COLOR_BGR2GRAY);

    Rect face = getFace(&frame_gray);

    if (face.width > 0) {

        // Pour toujours avec une nombre constant de faces, enlever une face puis la remplacer avec la plus récente
        faces.emplace_back(face);

        if (faces.size() < NUM_AVG_OVER_FRAMES) {
            faces.erase(faces.begin());


            Rect average_face = getAverageFace();

            // Draw green rectangle
            rectangle(frame2, average_face, Scalar(0,255,0), 2);

            // Detect Motion //
            // Extract working rect in frame2 and convert to gray
            cv::cvtColor(Mat(frame2, average_face), frameRect2, COLOR_BGR2GRAY);

            // Extract template image in frame1
            Mat templateImage(frameRect1, templateRect);

            // Create the matchTemplate image result
            Mat resultImage;    // to store the matchTemplate result
            int result_cols =  frame1.cols-templateWidth  + 1;
            int result_rows = frame1.rows-templateHeight + 1;
            resultImage.create(result_cols, result_rows, CV_32FC1);
            // Do the Matching between the working rect in frame2 and the templateImage in frame1
            matchTemplate(frameRect2, templateImage, resultImage, TM_CCORR_NORMED);

            // Localize the best match with minMaxLoc
            double minVal; double maxVal; Point minLoc; Point maxLoc;
            minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc);

            // Compute the translation vector between the origin and the matching rect
            Point vect(maxLoc.x-templateRect.x, maxLoc.y-templateRect.y);

            // Draw green rectangle and the translation vector
            rectangle(frame2, average_face, Scalar(0, 255, 0), 2);
            Point faceCenter(average_face.x + average_face.width / 2, average_face.y + average_face.height / 2);
            Point p(faceCenter.x+vect.x, faceCenter.y+vect.y);
            arrowedLine(frame2, faceCenter, p, Scalar(255, 255, 255), 2);

            // Draw the direction
            forward_mvmt += vect.y;
            side_mvmt += vect.x;
            std::cout << vect.x << " " << vect.y << std::endl;
            Point dirCenter(frameWidth / 2, frameHeight - 20);
            Point dirEdge(dirCenter.x + side_mvmt, dirCenter.y + forward_mvmt);
            arrowedLine(frame2, dirCenter, dirEdge, Scalar(0, 0, 255), 3);

            // Swap matrixes
            swap(frameRect1, frameRect2);
        }
    }

    // Display frame2
    imshow("WebCam", frame2);
}


Rect FaceHandler::getFace(Mat* frame_gray) {
    std::vector<Rect> faces;

    std::vector<int> rejectLevels;
    std::vector<double> levelWeights;

    face_cascade.detectMultiScale(*frame_gray, faces, rejectLevels, levelWeights, 1.1, 3, 0, Size(60, 60), Size(), true); //, 0, Size(60, 60), Size(300, 300));

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

