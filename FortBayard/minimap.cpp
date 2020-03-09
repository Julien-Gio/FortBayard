#include "minimap.h"
#include <iostream>

MiniMap::MiniMap(QWidget *parent) : QWidget(parent)
{
    // Connexion du timer
    connect(&m_AnimationTimer,  &QTimer::timeout, [&] {
        update();
    });

    m_AnimationTimer.setInterval(10);
    m_AnimationTimer.start();
    update();
}

void MiniMap::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    painter.setBrush(Qt::blue);
    painter.setPen(QPen(Qt::blue, 8));
    const float size = 30;
    const float middleX = this->size().width()/2.f, middleY = this->size().height()/2.f;
    painter.drawEllipse(middleX - size/2, middleY - size/2, size, size);

    const float length = 40;
    painter.drawLine(middleX, middleY, middleX + length * cos(rotation), middleY + length * sin(rotation));
}

float MiniMap::getRotation(){
    return rotation;
}

void MiniMap::setRotation(float r){
    rotation = r;
}
