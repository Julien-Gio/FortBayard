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
