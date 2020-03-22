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
const float MAX_DIMENSION     = 50.0f;


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
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL()
{
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LIGHT0);
}

void MyGLWidget::paintEvent(QPaintEvent *event)
{
    makeCurrent();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reinitialisation des tampons
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Definition de la position de la camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    maze.display();

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

    gluPerspective(70, WIN_WIDTH/(float)WIN_HEIGHT, 0.1, 40);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Fonction de gestion d'interactions clavier
void MyGLWidget::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {
        // Activation/Arret de l'animation
        case Qt::Key_Enter:
        {
            if(m_AnimationTimer.isActive())
                m_AnimationTimer.stop();
            else
                m_AnimationTimer.start();

            break;
        }

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
