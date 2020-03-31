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
    void setCamera();
    void rotate(float);
    float getRotation();
    void drawPlayer(QPainter *, float sizeOfCase, QPoint offset);
    float getPosX(){return posX;}
    float getPosY(){return posY;}
    void setPosition(float x, float y);
    void addFootPrint();
    void displayFootPrint();
};

#endif // PLAYER_H
