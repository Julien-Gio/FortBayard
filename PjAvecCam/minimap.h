#ifndef MINIMAP_H
#define MINIMAP_H

#include <QWidget>
#include <QPainter>
#include <math.h>
#include <QTimer>
#include <QKeyEvent>

class MiniMap : public QWidget
{
    Q_OBJECT
    float rotation = 0; // A supprimer quand la caméra fonctionnera
    QTimer m_AnimationTimer;
public:
    explicit MiniMap(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void setRotation(float);
    float getRotation();
signals:

};

#endif // MINIMAP_H
