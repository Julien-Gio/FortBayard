#define _USE_MATH_DEFINES

#include "myglwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <cmath>
#include <random>


// Declarations des constantes
const unsigned int WIN_WIDTH  = 1600;
const unsigned int WIN_HEIGHT = 900;
const float MAX_DIMENSION     = 50.0f;


// Constructeur
MyGLWidget::MyGLWidget(QWidget * parent) : QGLWidget(parent)
{
    // Reglage de la taille/position
    setFixedSize(WIN_WIDTH, WIN_HEIGHT);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    // Connexion du timer
    connect(&m_AnimationTimer,  &QTimer::timeout, [&] {
        m_TimeElapsed += 1.0f / 12.0f;
        updateGL();
    });

    m_AnimationTimer.setInterval(10);
    m_AnimationTimer.start();
}

MyGLWidget::~MyGLWidget(){

}

// Fonction d'initialisation
void MyGLWidget::initializeGL()
{
    // Reglage de la couleur de fond
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    // Activation du zbuffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}


// Fonction de redimensionnement
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


// Fonction d'affichage
void MyGLWidget::paintGL()
{
    // Reinitialisation des tampons
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Definition de la position de la camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    player.setCamera();
    maze.display();
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
            player.rotate(10 * 3.14159 / 180);
            break;
        }

        case Qt::Key_Left:
        {
            player.rotate(-10 * 3.14159 / 180);
            break;
        }

        case Qt::Key_Up:
        {
            player.walk(0.1);
            break;
        }

        case Qt::Key_Down:
        {
            player.walk(-0.1);
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
    updateGL();
}

int random(int m){
    return rand() % m;
}
