#include "player.h"

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
}

float Player::getRotation(){
    return yRotation;
}

void Player::drawPlayer(QPainter * painter){
    painter->setBrush(Qt::blue);
    painter->setPen(QPen(Qt::blue, 2));
    const float size = 10;
    float mapPosX = posX + 10, mapPosY = posY + 10;
    painter->drawEllipse(mapPosX - size/2, mapPosY - size/2, size, size);

    const float length = 13;
    painter->drawLine(mapPosX, mapPosY, mapPosX + length * cos(yRotation), mapPosY + length * sin(yRotation));
}
