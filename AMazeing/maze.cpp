/** An implementation of Prim's algorithm for generating mazes.
 * from <http://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm>
 *
 * C++ implementation by C. Ducottet
**/

#include<iostream>
#include <vector>
#include <ctime>
#include "cell.h"
#include "maze.h"

using namespace std;

Maze::Maze(int width, int height)
    : grid_(height,vector<Cell>(width)), width_(width), height_(height)
{
    generate();
}

void Maze::reinit()
{
    grid_=vector<vector<Cell>>(height_,vector<Cell>(width_));
}

void Maze::addFrontier(Point p, list<Point> &frontier)
{
    if (p.first>=0 && p.second>=0 && p.second<height_ && p.first<width_
            && grid_[p.second][p.first].getValue()==0) {
        grid_[p.second][p.first].setValue(Cell::FRONTIER);
        frontier.push_back(p);
    }
}


void Maze::mark(Point p, list<Point> &frontier)
{
    grid_[p.second][p.first].setValue(Cell::MARKED);
    addFrontier(Point(p.first-1, p.second),frontier);
    addFrontier(Point(p.first+1, p.second),frontier);
    addFrontier(Point(p.first, p.second-1),frontier);
    addFrontier(Point(p.first, p.second+1),frontier);
}


list<Point> Maze::neighbors(Point p)
{
    list<Point> n;
    if (p.first>0 && grid_[p.second][p.first-1].getValue()==Cell::MARKED)
        n.push_back(Point(p.first-1, p.second));
    if (p.first+1<width_ && grid_[p.second][p.first+1].getValue()==Cell::MARKED)
        n.push_back(Point(p.first+1, p.second));
    if (p.second>0 && grid_[p.second-1][p.first].getValue()==Cell::MARKED)
        n.push_back(Point(p.first, p.second-1));
    if (p.second+1<height_ && grid_[p.second+1][p.first].getValue()==Cell::MARKED)
        n.push_back(Point(p.first, p.second+1));
    return n;
}


Cell::Direction Maze::direction(Point f, Point t)
{
    if (f.first<t.first) return Cell::E;
    else if (f.first>t.first) return Cell::W;
    else if (f.second<t.second) return Cell::S;
    else return Cell::N;
}

void Maze::display(){

    float gray[4] = {0.8, 0.8, 0.8, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gray);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(-width_/2 * sizeByRoom, 0, -height_/2 * sizeByRoom); // Sol
        glVertex3f(-width_/2 * sizeByRoom, 0, height_/2 * sizeByRoom);
        glVertex3f(width_/2 * sizeByRoom, 0, height_/2 * sizeByRoom);
        glVertex3f(width_/2 * sizeByRoom, 0, -height_/2 * sizeByRoom);

        glNormal3f(0, -1, 0);
        glVertex3f(-width_/2 * sizeByRoom, wallHeight, -height_/2 * sizeByRoom); // Plafond
        glVertex3f(-width_/2 * sizeByRoom, wallHeight, height_/2 * sizeByRoom);
        glVertex3f(width_/2 * sizeByRoom, wallHeight, height_/2 * sizeByRoom);
        glVertex3f(width_/2 * sizeByRoom, wallHeight, -height_/2 * sizeByRoom);
    glEnd();

    float green[4] = {100.f/255, 200.f/255, 150.f/255, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
    /*Les murs du labyrinthe*/
    for (unsigned int i=0;i<height_;i++) {
        if(grid_[i][0].isFrontier(Cell::W))
            drawHorizontalWall(QPoint(- width_/2, i - height_/2), QPoint(- width_/2, i-height_/2 + 1));
        for (unsigned int j=0;j<width_;j++) {
            if (grid_[i][j].isFrontier(Cell::E))
                drawHorizontalWall(QPoint(j - width_/2 + 1, i - height_/2), QPoint(j - width_/2 + 1, i - height_/2 + 1));
            if (grid_[i][j].isFrontier(Cell::S))
                drawVerticalWall(QPoint(j - width_/2, i - height_/2 + 1), QPoint(j - width_/2 + 1, i - height_/2 + 1));
        }
    }
    for (unsigned int j=0;j<width_;j++) {
        if (grid_[0][j].isFrontier(Cell::N))
            drawVerticalWall(QPoint(j - width_/2, - height_/2), QPoint(j - width_/2 + 1, - height_/2));
    }
}

void Maze::drawVerticalWall(QPoint a, QPoint b){
    a = a * sizeByRoom;
    b = b * sizeByRoom;
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(a.rx() + wallDepth/2, 0, a.ry() - wallDepth/2);
        glVertex3f(a.rx() + wallDepth/2, wallHeight, a.ry() - wallDepth/2);
        glVertex3f(b.rx() + wallDepth/2, wallHeight, b.ry() - wallDepth/2);
        glVertex3f(b.rx() + wallDepth/2, 0, b.ry() - wallDepth/2);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(b.rx() + wallDepth/2, wallHeight, b.ry() - wallDepth/2);
        glVertex3f(b.rx() + wallDepth/2, 0, b.ry() - wallDepth/2);
        glVertex3f(b.rx() + wallDepth/2, 0, b.ry() + wallDepth/2);
        glVertex3f(b.rx() + wallDepth/2, wallHeight, b.ry() + wallDepth/2);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(b.rx() + wallDepth/2, 0, b.ry() + wallDepth/2);
        glVertex3f(b.rx() + wallDepth/2, wallHeight, b.ry() + wallDepth/2);
        glVertex3f(a.rx() + wallDepth/2, wallHeight, a.ry() + wallDepth/2);
        glVertex3f(a.rx() + wallDepth/2, 0, a.ry() + wallDepth/2);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(a.rx() + wallDepth/2, wallHeight, a.ry() + wallDepth/2);
        glVertex3f(a.rx() + wallDepth/2, 0, a.ry() + wallDepth/2);
        glVertex3f(a.rx() + wallDepth/2, 0, a.ry() - wallDepth/2);
        glVertex3f(a.rx() + wallDepth/2, wallHeight, a.ry() - wallDepth/2);
    glEnd();
}

void Maze::drawHorizontalWall(QPoint a, QPoint b){
    a = a * sizeByRoom;
    b = b * sizeByRoom;
    glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(a.rx() - wallDepth/2, 0, a.ry() - wallDepth/2);
        glVertex3f(a.rx() - wallDepth/2, wallHeight, a.ry() - wallDepth/2);
        glVertex3f(b.rx() - wallDepth/2, wallHeight, b.ry() + wallDepth/2);
        glVertex3f(b.rx() - wallDepth/2, 0, b.ry() + wallDepth/2);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(b.rx() - wallDepth/2, wallHeight, b.ry() + wallDepth/2);
        glVertex3f(b.rx() - wallDepth/2, 0, b.ry() + wallDepth/2);
        glVertex3f(b.rx() + wallDepth/2, 0, b.ry() + wallDepth/2);
        glVertex3f(b.rx() + wallDepth/2, wallHeight, b.ry() + wallDepth/2);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(b.rx() + wallDepth/2, 0, b.ry() + wallDepth/2);
        glVertex3f(b.rx() + wallDepth/2, wallHeight, b.ry() + wallDepth/2);
        glVertex3f(a.rx() + wallDepth/2, wallHeight, a.ry() - wallDepth/2);
        glVertex3f(a.rx() + wallDepth/2, 0, a.ry() - wallDepth/2);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(a.rx() + wallDepth/2, wallHeight, a.ry() - wallDepth/2);
        glVertex3f(a.rx() + wallDepth/2, 0, a.ry() - wallDepth/2);
        glVertex3f(a.rx() - wallDepth/2, 0, a.ry() - wallDepth/2);
        glVertex3f(a.rx() - wallDepth/2, wallHeight, a.ry() - wallDepth/2);
    glEnd();
}

void Maze::generate()
{
    list<Point> frontier;

    // Initialize cells if the maze was already generated
    reinit();

    // Initialize random generator
    srand (time(NULL));

    // Mark a random cell and add the frontier cells to the list
    mark(Point(rand() % width_, rand() % height_),frontier);

    while(!frontier.empty()) {

        // Take a random frontier cell f (from)
        auto randPos=frontier.begin();
        advance(randPos,rand() % frontier.size());
        Point f=*randPos;
        frontier.erase(randPos);

        // Take a random neighbor t (to) of that cell
        list<Point> n=neighbors(f);
        randPos=n.begin();
        advance(randPos,rand() % n.size());
        Point t=*randPos;

        // Carve a passage from f to t
        Cell::Direction d=direction(f,t);
        grid_[f.second][f.first].setFrontier(d,false);
        grid_[t.second][t.first].setFrontier(Cell::Direction((d+2)%4),false);

        // Mark the cell and add the frontier cells to the list
        mark(f,frontier);
    }
}
