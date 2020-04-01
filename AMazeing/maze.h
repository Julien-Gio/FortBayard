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
    ~Collectible();
    virtual void display(float);
    void setPosition(float x, float y){posX = x; posY = y;}
    virtual void collected(){hasBeenCollected = true;}
    bool HasBeenCollected(){return hasBeenCollected;}
    void destroyIt(){hasBeenCollected = true;}
    virtual void thisObjectHasBeenCollected(Collectible*){}
    float getX(){return posX;}
    float getY(){return posY;}
};

class Maze
{
    std::vector<Collectible*> collectibles;
    Player player;

    vector<vector<Cell>> grid_;

    struct Wall{
        QPoint debut, fin;
    };

    QElapsedTimer timer;

    std::vector<Wall> walls;
    bool isPlayerMoving = false;

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
    Maze(int width = 10, int height = 6);
    void reinit();
    void display();
    void generate();
    void drawMap(QPainter*);
    float getSizeOfRoom(){return sizeByRoom;}

    void init();
    bool tryFrontier(int, int, Cell::Direction);
    void rotate(float);
    void walk(float);
    void idle(){isPlayerMoving = false;}
    void removeWall();

private:
    void drawVerticalWall(QPoint, QPoint);
    void drawHorizontalWall(QPoint, QPoint);
};

// La clé qui permet d'ouvrir une porte dans le labyrinthe
class Key : public Collectible{
    bool seeThroughWall = false;
    Maze * maze;
public:
    Key(Maze*);
    void collected() override;
    void display(float) override;
    void seeThrough(){seeThroughWall=true;}
    void thisObjectHasBeenCollected(Collectible *) override{}
};

// Les lunettes qui permettent de voir la clé à travers les murs
class Glasses : public Collectible{
    Key* key;
public:
    Glasses(Key*);
    void collected() override;
    void display(float) override;
    void thisObjectHasBeenCollected(Collectible*) override;
};

//Fonction permettant de transformer un int sous format horaire sur une alarme par exemple 8 => "08" et 12 => "12"
QString intToStringHourFormat(long int);

#endif // MAZE_H
