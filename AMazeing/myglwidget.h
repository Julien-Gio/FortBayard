#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QGLWidget>
#include <QTimer>
#include "player.h"
#include "maze.h"
#include "facehandler.h"

class Bubble;
class QtLogo;
class QPaintEvent;
class QWidget;

class MyGLWidget : public QGLWidget
{
    Q_OBJECT

    Maze maze;
    FaceHandler fh;

public:
    MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintEvent(QPaintEvent *event);
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent*);

private:
    QTimer m_AnimationTimer;
};

#endif
