#ifndef PLAYER_H
#define PLAYER_H


#include <qopengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <QPainter>
#include <QGLWidget>

struct FootPrint{
    float posX, posY, rotation;
    bool isRightFoot = true;
};

class Player
{
    const float HEIGHT = 1.8;
    const float printSize = 0.2;
    float yRotation = 0;
    float posX = 1, posY = 1; // En fait c'est posX et posZ mais bon c'est plus explicite comme ça je trouve

    GLuint footPrintTexture;

    std::vector<FootPrint> footPrints;

public:
    Player();
    void init();
    void setCamera(); // Role: Positionne la caméra à la position du joueur.
    void rotate(float); // Role: Rotate le joueur. Entrée: l'amplitude de la rotation
    float getRotation();
    void drawPlayer(QPainter *, float sizeOfCase, QPoint offset); // Role: Dessine le joueur en 2D sur la minimap. Entrée: Le QPainter pour dessiner, la taille d'une case (ce qui va influer sur le positionnement du dessin et l'offset de la position de la minimap
    float getPosX(){return posX;}
    float getPosY(){return posY;}
    void setPosition(float x, float y);
    void addFootPrint(); // Role: Ajoute une trace de pas à la position actuel du joueur.
    void displayFootPrint(); // Role: Affiche toutes les traces de pas dans l'environnement 3D.
};

#endif // PLAYER_H
