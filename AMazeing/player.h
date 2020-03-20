#ifndef PLAYER_H
#define PLAYER_H


#include <qopengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <QPainter>


class Player
{
    const float HEIGHT = 1.8;
    float yRotation = 0;
    float posX = 1, posY = 1; // En fait c'est posX et posZ mais bon c'est plus explicite comme ça je trouve
public:
    Player();
    void setCamera();
    void rotate(float);
    void walk(float);
    float getRotation();
    void drawPlayer(QPainter *);
};

#endif // PLAYER_H
