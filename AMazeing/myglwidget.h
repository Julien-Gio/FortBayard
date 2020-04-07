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

/*
 * Classe qui gère a la fois le labyrinthe, son l'affichage 3D et fait le lien entre
 * la classe de detection de mouvement du visage et le joueur.
 **/

class MyGLWidget : public QGLWidget
{
    Q_OBJECT

    Maze maze;
    FaceHandler fh;
    bool isGameFinished = false;
    QString score;


public:
    MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();

    void setMainWindow(MainWindow* p) { fh.setMainWindowP(p); }

protected:
    void initializeGL();
    void paintEvent(QPaintEvent *event);
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void drawTextWithStroke(int x, int y, QString, int fontSize);

private:
    QTimer m_AnimationTimer;

public slots:
    void GameIsFinished(QString);
};

#endif
