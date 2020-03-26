#include "player.h"
#include <iostream>

Player::Player()
{

}

void Player::init(){
    QString path = QString(":/Footprint.png");
    QImage image = QGLWidget::convertToGLFormat(QImage(path));
    glGenTextures(1, &footPrintTexture);
    glBindTexture(GL_TEXTURE_2D, footPrintTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
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

float Player::getRotation(){
    return yRotation;
}

void Player::drawPlayer(QPainter * painter, float sizeOfCase, QPoint offset){
    painter->setBrush(Qt::green);
    painter->setPen(QPen(Qt::green, sizeOfCase/5));
    const float size = sizeOfCase /2;
    const float mapPosX = posX*sizeOfCase + offset.x(), mapPosY = posY*sizeOfCase + offset.y();
    painter->drawEllipse(mapPosX - size/2, mapPosY - size/2, size, size);

    const float length = size * 1.3;
    painter->drawLine(mapPosX, mapPosY, mapPosX + length * cos(yRotation), mapPosY + length * sin(yRotation));
}

void Player::setPosition(float x, float y){
    static float distSinceLastPrint = 10;
    if(distSinceLastPrint > 0.5){
        distSinceLastPrint = 0;
        addFootPrint();
    }else{
        distSinceLastPrint += std::sqrt(std::pow(x - posX, 2) + std::pow(y - posY, 2));
    }
    posX = x;
    posY = y;
}

void Player::addFootPrint(){
    static bool rightFoot = true;
    footPrints.emplace_back();
    footPrints.back().rotation = yRotation * 180 / 3.14159;
    footPrints.back().isRightFoot = rightFoot;
    rightFoot = !rightFoot;
    if(rightFoot){
        footPrints.back().posX = posX - printSize * std::sin(yRotation);
        footPrints.back().posY = posY + printSize * std::cos(yRotation);
    }else{
        footPrints.back().posX = posX + printSize * std::sin(yRotation);
        footPrints.back().posY = posY - printSize * std::cos(yRotation);
    }
}

void Player::displayFootPrint(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, footPrintTexture); // On définit la texture courante
    for(unsigned int i = 0; i < footPrints.size(); i++){
        glPushMatrix();
        glTranslated(footPrints[i].posX, 0, footPrints[i].posY);
        glRotated(180-footPrints[i].rotation, 0, 1, 0);
        glBegin(GL_QUADS);
            glTexCoord2f(0 + footPrints[i].isRightFoot * 0.5, 1); glVertex3f(-printSize, 0.01, -printSize/2);
            glTexCoord2f(1 - !footPrints[i].isRightFoot * 0.5, 1); glVertex3f(-printSize, 0.01, printSize/2);
            glTexCoord2f(1 - !footPrints[i].isRightFoot * 0.5, 0); glVertex3f(printSize, 0.01, printSize/2);
            glTexCoord2f(0 + footPrints[i].isRightFoot * 0.5, 0); glVertex3f(printSize, 0.01, -printSize/2);
        glEnd();
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}




