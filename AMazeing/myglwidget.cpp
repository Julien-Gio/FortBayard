#define _USE_MATH_DEFINES
#include <QtGui>
#include <QtOpenGL>
#include <stdlib.h>

#include <math.h>
#include "myglwidget.h"
#include <GL/glu.h>

#include <iostream>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


// Declarations des constantes
const unsigned int WIN_WIDTH  = 1400;
const unsigned int WIN_HEIGHT = 800;


MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), maze()
{
    // Reglage de la taille/position
    setFixedSize(WIN_WIDTH, WIN_HEIGHT);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    // Connexion du timer
    connect(&m_AnimationTimer,  &QTimer::timeout, [&] {
        update();
    });

    m_AnimationTimer.setInterval(10);
    m_AnimationTimer.start();
    setAutoFillBackground(false);

    connect(&maze, SIGNAL(endOfGame(QString)), this, SLOT(GameIsFinished(QString)));
}

void MyGLWidget::init(bool playWithCamer){
    playWithCamera = playWithCamer;
    if(playWithCamera)
        fh = new FaceHandler(&maze);

}

MyGLWidget::~MyGLWidget()
{
    if(playWithCamera)
        delete fh;
}

void MyGLWidget::initializeGL()
{
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LIGHT0);
    maze.init();
}

void MyGLWidget::paintEvent(QPaintEvent *event)
{
    makeCurrent();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // Reinitialisation des tampons
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    // Definition de la position de la camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    maze.display();

    if(isGameFinished){
        drawTextWithStroke(rect().width()/2 - 140, rect().height()/2 - 20, "Félicitations !", 32);
        drawTextWithStroke(rect().width()/2 - 360, rect().height()/2 + 20, "Tu as fais un score de " + score, 32);
        drawTextWithStroke(rect().width()/2 - 240, rect().height()/2 + 85, "Appuyez sur Entrée pour recommencer", 24);
        drawTextWithStroke(rect().width()/2 - 220, rect().height()/2 + 130, "Appuyez sur Echap pour quitter", 24);
    }

    glShadeModel(GL_FLAT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    QPainter painter(this);
    maze.drawMap(&painter);
    painter.end();
}

void MyGLWidget::resizeGL(int width, int height)
{
    // Definition du viewport (zone d'affichage)
    glViewport(0, 0, width, height);

    // Definition de la matrice de projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(70, WIN_WIDTH/(float)WIN_HEIGHT, 0.01, 40);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Fonction de gestion d'interactions clavier
void MyGLWidget::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {
        // Sortie de l'application
        case Qt::Key_Escape:
            if(isGameFinished) {
                emit goToMainMenu();
            }
            break;

        case Qt::Key_Right:
            maze.rotate(10 * 3.14159 / 180);
            break;

        case Qt::Key_Left:
            maze.rotate(-10 * 3.14159 / 180);
            break;

        case Qt::Key_Up:
            maze.walk(0.1);
            break;

        case Qt::Key_Down:
            maze.walk(-0.1);
            break;

        case Qt::Key_Enter: case Qt::Key_Return:
            if(isGameFinished){
                maze.init();
                isGameFinished = false;
            }
            break;

        // Cas par defaut
        default:
            // Ignorer l'evenement
            event->ignore();
            return;
    }

    // Acceptation de l'evenement et mise a jour de la scene
    event->accept();
    update();
}

// Fonction de gestion d'interactions clavier
void MyGLWidget::keyReleaseEvent(QKeyEvent * event)
{
    switch(event->key())
    {

        case Qt::Key_Down:
        case Qt::Key_Up:
        case Qt::Key_Left:
        case Qt::Key_Right:
        {
            maze.idle();
            break;
        }

        // Cas par defaut
        default:
        {
            // Ignorer l'evenement
            event->ignore();
            return;
        }
    }
    event->accept();
}

void MyGLWidget::drawTextWithStroke(int x, int y, QString str, int fontSize){
    QFont font;
    font.setPointSize(fontSize);
    glColor3ub(0, 0, 0);
    renderText(x-2, y-2, str, font);
    renderText(x+2, y-2, str, font);
    renderText(x+2, y+2, str, font);
    renderText(x-2, y+2, str, font);
    glColor3ub(255, 255, 255);
    renderText(x, y, str, font);
}

void MyGLWidget::GameIsFinished(QString time){
    isGameFinished = true;
    score = time;
}
