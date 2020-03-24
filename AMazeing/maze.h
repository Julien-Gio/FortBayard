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
#include <QGLWidget>

using namespace std;

using Point=pair<int,int>;


class Collectible{
    float RAYON = 0.4, ROTATION_SPEED = 20;
    bool hasBeenCollected = false;
    GLuint textId;
    GLUquadric* quadrique;
    float posX = 1, posY = 1, z = 1.5;
    float totalTime = 0;

public:
    Collectible(QString);
    ~Collectible();
    void display(float);
    void setPosition(float x, float y){posX = x; posY = y;}
    virtual void collected(){}
    float getX(){return posX;}
    float getY(){return posY;}
};

class Maze
{
    std::vector<Collectible*> collectibles;
    Player player;

    vector<vector<Cell>> grid_;

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
    Maze(int width = 10, int height = 10);
    void reinit();
    void display();
    void generate();
    void drawMap(QPainter*);
    float getSizeOfRoom(){return sizeByRoom;}

    void init();
    bool tryFrontier(int, int, Cell::Direction);
    void rotate(float);
    void walk(float);
    void removeWall();

private:
    void drawVerticalWall(QPoint, QPoint);
    void drawHorizontalWall(QPoint, QPoint);
};

class Key : public Collectible{
    Maze * maze;
public:
    Key(Maze*);
    void collected();
};

#endif // MAZE_H
