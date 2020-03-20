#include "player.h"
#include <iostream>

Player::Player()
{

}

void Player::setCamera(){
    gluLookAt(posX, HEIGHT, posY, posX + std::cos(yRotation), HEIGHT, posY + std::sin(yRotation), 0, 1, 0);
    float position[4] = {posX, HEIGHT, posY, 1};
    float direction[3] = {0, 0, -1};
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void Player::rotate(float r){
    yRotation += r;
}

void Player::walk(float w){
    posX += w * std::cos(yRotation);
    posY += w * std::sin(yRotation);
    std::cout<<posX<<"   "<<posY<<std::endl;
}

float Player::getRotation(){
    return yRotation;
}

void Player::drawPlayer(QPainter * painter, float sizeOfCase, QPoint offset){
    painter->setBrush(Qt::green);
    painter->setPen(QPen(Qt::green, sizeOfCase/5));
    const float size = sizeOfCase /2;
    const float mapPosX = posX*sizeOfCase + offset.rx(), mapPosY = posY*sizeOfCase + offset.ry();
    painter->drawEllipse(mapPosX - size/2, mapPosY - size/2, size, size);

    const float length = size * 1.3;
    painter->drawLine(mapPosX, mapPosY, mapPosX + length * cos(yRotation), mapPosY + length * sin(yRotation));
}
