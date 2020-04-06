#define _USE_MATH_DEFINES
#include <QtGui>
#include <QtOpenGL>
#include <stdlib.h>

#include <math.h>
#include "myglwidget.h"
#include <GL/glu.h>


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


// Declarations des constantes
const unsigned int WIN_WIDTH  = 1600;
const unsigned int WIN_HEIGHT = 900;


MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
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

MyGLWidget::~MyGLWidget()
{
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
        QFont font;
        font.setPointSize(font.pointSize()*3);
        renderText(rect().width()/2, rect().height()/2, "Félicitations !!!", font);
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
        {
            exit(0);
        }

        case Qt::Key_Right:
        {
            maze.rotate(10 * 3.14159 / 180);
            break;
        }

        case Qt::Key_Left:
        {
            maze.rotate(-10 * 3.14159 / 180);
            break;
        }

        case Qt::Key_Up:
        {
            maze.walk(0.1);
            break;
        }

        case Qt::Key_Down:
        {
            maze.walk(-0.1);
            break;
        }

    case Qt::Key_Enter:
        {
            if(isGameFinished){
                maze.init();
                isGameFinished = false;
            }
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

void MyGLWidget::GameIsFinished(QString time){
    isGameFinished = true;
    score = time;
}
