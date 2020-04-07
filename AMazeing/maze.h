#ifndef MAZE_H
/** An implementation of Prim's algorithm for generating mazes.
 * from <http://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm>
 *
 * C++ implementation by C. Ducottet
**/

#define MAZE_H

#include "cell.h"
#include <vector>
#include <list>
#include <utility>
#include <QPoint>
#include<iostream>

#include <qopengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QPainter>
#include "player.h"
#include <QElapsedTimer>

using namespace std;

using Point=pair<int,int>;


// La classe Parent des objets que l'on peut retrouver dans le labyrinthe
class Collectible{
protected:
    float RAYON = 0.4, ROTATION_SPEED = 20;
    bool hasBeenCollected = false;
    GLuint textId;
    GLUquadric* quadrique;
    float posX = 1, posY = 1, z = 1.5;
    float totalTime = 0;

public:
    Collectible(QString);
    virtual ~Collectible(){}
    virtual void display(float); // Role: Affiche en 3D l'objet dans l'espace. Entrée: le temps écoulé depuis la dernière frame
    void setPosition(float x, float y){posX = x; posY = y;}
    virtual void collected(){hasBeenCollected = true;} // Role: Effectuer une action sur un autre objet et mettre à true le booleen hasBeenCollected
    bool HasBeenCollected(){return hasBeenCollected;}
    void destroyIt(){hasBeenCollected = true;} // Role: Mettre à true le booleen hasBeenCollected sans qu'il ai été ramassé
    virtual void thisObjectHasBeenCollected(Collectible*){} // Role: Signaler aux autres objets quel objet vient d'être ramasser pour pouvoir se détruire si c'est le'objet sur lequel ils devaient influer
    float getX(){return posX;}
    float getY(){return posY;}
};

class Maze : public QObject
{
    Q_OBJECT

    std::vector<Collectible*> collectibles;
    Player player;

    vector<vector<Cell>> grid_;

    struct Wall{
        QPoint debut, fin;
    };

    QElapsedTimer timer;

    std::vector<Wall> walls;
    bool isPlayerMoving = false;
    bool gameIsFinished = false;

    const float wallHeight = 3;
    const float wallDepth = 0.2;
    const float sizeByRoom = 2;

    const float sizeOfCaseOnMap = 30; // Value in pixel
    const QPoint offset = QPoint(10, 10);

    int width_;
    int height_;

    void addFrontier(Point p,list<Point> & frontier);
    void mark(Point p,list<Point> & frontier);
    list<Point> neighbors(Point p);
    Cell::Direction direction(Point f, Point t);

public:
    Maze();
    void reinit(); // Role: réinitialiser le labyrinthe
    void display(); // Role: Afficher en 3D le labyrinthe
    void generate(); // Role: génerer un labyrinthe
    void drawMap(QPainter*); // Role: afficher en 2D le labyrinthe en haut à gauche de l'écran. Entrée: le QPainter pour dessiner
    float getSizeOfRoom(){return sizeByRoom;}

    void init(); // Role: Initialise les objets, et le personnage à des positions aléatoire avant de générer le labyrinthe
    bool tryFrontier(int, int, Cell::Direction); // Role: Pour une certaine case, essaye si il y a un mur dans une certaine direction. Entrée: x et y la position de la case et Cell::Direction le direction à tester
    void rotate(float); // Role: Rotate le personnage à l'intérieur du labyrinthe. Entrée: L'angle de rotation en radian.
    void walk(float); // Role: Fais marcher le personnage dans le labyrinthe. Entrée: La distance à parcourir.
    void idle(){isPlayerMoving = false;} // Role: Signaler que le personnage ne doit pas bouger (ce qui permet de rafficher la minimap).
    void removeWall(); // Role: Retire un mur sur le contour du labyrinthe de façon aléatoire.


signals:
    void endOfGame(QString);


private:
    void drawVerticalWall(QPoint, QPoint); // Role: Dessine un mur vertical en 3D d'un point A à un point B. Entrée: Le point A et le point B
    void drawHorizontalWall(QPoint, QPoint); // Role: Dessine un mur horizontal en 3D d'un point A à un point B. Entrée: Le point A et le point B
};

// La clé qui permet d'ouvrir une porte dans le labyrinthe
class Key : public Collectible{
    bool seeThroughWall = false;
    Maze * maze;
public:
    Key(Maze*);
    ~Key(){gluDeleteQuadric(quadrique);}
    void collected() override; // Role: La fonction appelé quand l'objet est collecté.
    void display(float) override; // Role: La fonction pour afficher en 3D l'objet dans la labyrinthe. Entrée: le temps écoulé depuis la dernière frame
    void seeThrough(){seeThroughWall=true;} // Role: Une fonction permettant d'activer le fait que l'on puisse voir l'objet à travers les murs
    void thisObjectHasBeenCollected(Collectible *) override{} // Role: La fonction permettant de signaler à cette objet, quel objet a été ramassé.
};

// Les lunettes qui permettent de voir la clé à travers les murs
class Glasses : public Collectible{
    Key* key;
public:
    Glasses(Key*);
    ~Glasses(){}
    void collected() override; // Role: La fonction appelé quand l'objet est collecté.
    void display(float) override; // Role: La fonction pour afficher en 3D l'objet dans la labyrinthe. Entrée: le temps écoulé depuis la dernière frame
    void thisObjectHasBeenCollected(Collectible*) override; // Role: La fonction permettant de signaler à cette objet, quel objet a été ramassé. Pour le cas des lunettes, si la clé a déjà été ramassé, alors on supprime les lunettes
};

//Fonction permettant de transformer un int sous format horaire sur une alarme par exemple 8 => "08" et 12 => "12"
QString intToStringHourFormat(long int);

#endif // MAZE_H
