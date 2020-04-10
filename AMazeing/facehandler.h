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

class MainWindow;  // Forward declaration

class FaceHandler {
    std::thread* th;
    Maze* maze;  // Passer dans le constructeur. FaceHandler ne créer/delete pas maze. Utile pour bouger controler le joueur
    MainWindow* mw = NULL;  // Reference vers le QLabel de la fenetre qui doit recevoir un feed de la camera

    // Quelques constantes pour rendre le code plus lisible //
    // Indices des directions dans le tableau SEUILS
    const int HAUT = 0;
    const int DROITE = 1;
    const int BAS = 2;
    const int GAUCHE = 3;
    const float SEUILS[4] = {3.0, 2.0, 2.0, 2.0};  // Dans l'ordre : Haut, droite, bas, gauche

    const float ROTATION_SPEED = 0.015;
    const float WALK_SPEED = 0.1;

    const int frameWidth = 320;
    const int frameHeight = 240;
    const float templateWidth = 0.4;  // Percentage of face
    const float templateHeight = 0.5;  // Percentage of face
    const float templateOffsetX = 0.0;  // Percentage of face. '= 0' centered, '< 0' to the left, '> 0' to the right
    const float templateOffsetY = 0.05;  // Percentage of face. < 0 above center, > 0 below center


    const unsigned int NUM_AVG_OVER_FRAMES = 10;  // Nombre de frames du visage en mémoire pour trouver la moyenne

    /* MACHINE D'ETAT :
     * 'C' : continously detecting faces
     * 'N' : currently no face is detected
     * 'J' : just detected a face
     *
     * TRANSITIONS :
     * C / J => N if no face is detected
     * N => J if a face is detected
     * J => C if faces are detected for long enough
     *
     * 'C' est le seul etat où les calculs de rotation de la tête sont faits.
    **/
    const char STATES[3] = {'C', 'J', 'N'};  // Etats possibles
    char state;  // Etat actuel

    std::vector<Rect> faces;  // Tableau avec les visages des n frames précedentes (nous permettant de faire un moyenne)
    VideoCapture cap;
    CascadeClassifier faceCascade;  // Objet pouvant trouver un visage

    Rect templateRect;
    Mat templateImage;  // Matrice avec le motif (zone autour du nez)
    Mat resultImage;    // Pour stocker le restulat de la X-corrélation

    Mat frame, frameRect;

    bool debugGraphics = true;

public:
    FaceHandler(Maze* maze_);  // Constructeur
    ~FaceHandler();  // Destructeur

    /* Role : récoupère un visage et appel updateC/J/N en fonction de l'état actuel. Gère les changement d'état aussi. */
    void update();

    /* Role : active ou desactive la visualisation du debuggage. */
    void toogle_debug_graphics() {
        debugGraphics = !debugGraphics;
    }

    void setMainWindowP(MainWindow* p) { mw = p; }

private:
    void updateC(Rect& face);  // Rôle : méthode de mise à jour de l'etat 'C'.
    void updateJ(Rect& face);  // Rôle : méthode de mise à jour de l'etat 'J'.
    void updateN(Rect& face);  // Rôle : méthode de mise à jour de l'etat 'N'.

    /* Rôle : Utilise la méthode detectMultiScale de la classe CascadeClassifieur pour extraire le visage de l'image.
     * Sortie : un Rect donnant la position du visage dans cette frame.
     * */
    Rect getFace(Mat* frame_gray);

    /* Rôle : calcul la position moyenne du visage sur les n dernières frames.
     * Sortie : un Rect avec la position moyenne du visage. */
    Rect getAverageFace();

};

#endif // FACEHANDLER_H
