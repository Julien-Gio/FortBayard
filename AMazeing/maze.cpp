/** An implementation of Prim's algorithm for generating mazes.
 * from <http://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm>
 *
 * C++ implementation by C. Ducottet
**/

#include <vector>
#include <ctime>
#include "cell.h"
#include "maze.h"

using namespace std;

Maze::Maze(int width, int height)
    : grid_(height,vector<Cell>(width)), width_(width), height_(height)
{
}

void Maze::init(){
    generate(); // Génere le labyrinthe
    collectibles.emplace_back(new Key(this)); // Crée une clé que l'on positionne ensuite aléatoirement
    collectibles.back()->setPosition((int)(sizeByRoom * rand())%width_ + 1, (int)(sizeByRoom * rand())%height_ + 1);
    collectibles.emplace_back(new Glasses(dynamic_cast<Key*>(collectibles[0]))); // Crée des lunettes que l'on positionne ensuite aléatoirement
    collectibles.back()->setPosition((int)(sizeByRoom * rand())%width_ + 1, (int)(sizeByRoom * rand())%height_ + 1);
    player.init(); // On repositionne le joueur dans le labyrinthe
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    player.setCamera();

    float gray[4] = {0.8, 0.8, 0.8, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gray);
    // Dessine le sole et la plafond
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(0, 0, 0); // Sol
        glVertex3f(0, 0, height_ * sizeByRoom);
        glVertex3f(width_ * sizeByRoom, 0, height_ * sizeByRoom);
        glVertex3f(width_ * sizeByRoom, 0, 0);

        glNormal3f(0, -1, 0);
        glVertex3f(0, wallHeight, 0); // Plafond
        glVertex3f(0, wallHeight, height_ * sizeByRoom);
        glVertex3f(width_ * sizeByRoom, wallHeight, height_ * sizeByRoom);
        glVertex3f(width_ * sizeByRoom, wallHeight, 0);
    glEnd();

    float green[4] = {100.f/255, 200.f/255, 150.f/255, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
    /*Les murs du labyrinthe*/
    for (unsigned int i=0;i<height_;i++) {
        if(grid_[i][0].isFrontier(Cell::W))
            drawHorizontalWall(QPoint(0, i), QPoint(0, i + 1));
        for (unsigned int j=0;j<width_;j++) {
            if (grid_[i][j].isFrontier(Cell::E))
                drawHorizontalWall(QPoint(j + 1, i), QPoint(j + 1, i + 1));
            if (grid_[i][j].isFrontier(Cell::S))
                drawVerticalWall(QPoint(j, i + 1), QPoint(j + 1, i + 1));
        }
    }
    for (unsigned int j=0;j<width_;j++) {
        if (grid_[0][j].isFrontier(Cell::N))
            drawVerticalWall(QPoint(j, 0), QPoint(j + 1, 0));
    }

    // On affiche les différents objets
    for(unsigned int i = 0; i < collectibles.size(); i++){
        collectibles[i]->display(10);
    }

    glDisable(GL_BLEND);
    player.displayFootPrint();
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
    timer.start();
}

void Maze::drawMap(QPainter *painter){
    if(!isPlayerMoving){ // Si le joueur ne bouge pas, alors on affiche la minimap
        painter->fillRect(offset.x(), offset.y(), sizeOfCaseOnMap * width_, sizeOfCaseOnMap * height_, QBrush(QColor(0, 0, 0, 120)));
        painter->setBrush(Qt::white);
        painter->setPen(QPen(Qt::white, 2));
        for (unsigned int i=0;i<height_;i++) {
            if(grid_[i][0].isFrontier(Cell::W))
                painter->drawLine(sizeOfCaseOnMap*QPoint(0, i) + offset, sizeOfCaseOnMap*QPoint(0, i + 1) + offset);
            for (unsigned int j=0;j<width_;j++) {
                if (grid_[i][j].isFrontier(Cell::E)){
                    painter->drawLine(sizeOfCaseOnMap*QPoint(j + 1, i) + offset, sizeOfCaseOnMap*QPoint(j + 1, i + 1) + offset);
                }
                if (grid_[i][j].isFrontier(Cell::S))
                    painter->drawLine(sizeOfCaseOnMap*QPoint(j, i + 1) + offset, sizeOfCaseOnMap*QPoint(j + 1, i + 1) + offset);
            }
        }
        for (unsigned int j=0;j<width_;j++) {
            if (grid_[0][j].isFrontier(Cell::N))
                painter->drawLine(sizeOfCaseOnMap*QPoint(j, 0) + offset, sizeOfCaseOnMap*QPoint(j + 1, 0) + offset);
        }

        player.drawPlayer(painter, sizeOfCaseOnMap/sizeByRoom, offset);
    }

    // Ici on affiche la chronomètre à droite de la minimap (ou de l'endroit ou elle serait affiché si elle ne l'est pas)
    QFont font = painter->font() ;
    font.setPointSize(font.pointSize() * 2);
    painter->setFont(font);

    painter->setPen(QPen(Qt::white, 20));
    long int time = timer.elapsed();
    QString hour;
    hour.append(intToStringHourFormat(time/60000) + " : ");
    time -= (time/60000)*60000;
    hour.append(intToStringHourFormat(time/1000) + " : ");
    time -= (time/1000)*1000;
    hour.append(intToStringHourFormat(time));
    painter->drawText(offset.x() + sizeOfCaseOnMap * width_ + 20, 4 * offset.y(), QString(hour));
}

bool Maze::tryFrontier(int x, int y, Cell::Direction d){
    if(x >= 0 && x < width_ && y >= 0 && y < height_){
        if(grid_[y][x].isFrontier(d))
            return true;
    }
    return false;
}

void Maze::rotate(float r){
    isPlayerMoving = true;
    player.rotate(r);
}

void Maze::walk(float w){
    isPlayerMoving = true;
    const float hitboxSize = 0.3;
    float newX = player.getPosX() + w * std::cos(player.getRotation());
    float newY = player.getPosY() +  w * std::sin(player.getRotation());

    // Si on est proche de la zone gauche de la case, on test si il y a un mur et si il y en a un, alors on recule le nouveau x au niveau de la parois du mur.
    if(std::fmod(newX, sizeByRoom) <= hitboxSize){
        if(tryFrontier(std::floor(newX/sizeByRoom), std::floor(newY/sizeByRoom), Cell::W)){
            newX = std::floor(newX/sizeByRoom)*sizeByRoom + hitboxSize;
        }
        // Si on est proche de la zone droite de la case, on test si il y a un mur et si il y en a un, alors on recule le nouveau x au niveau de la parois du mur.
    }else if(std::fmod(newX, sizeByRoom) >= sizeByRoom - hitboxSize){
        if(tryFrontier(std::floor(newX/sizeByRoom), std::floor(newY/sizeByRoom), Cell::E)){
            newX = std::floor(newX/sizeByRoom)*sizeByRoom + sizeByRoom - hitboxSize;
        }
    }
        // Si on est proche de la zone haute de la case, on test si il y a un mur et si il y en a un, alors on recule le nouveau x au niveau de la parois du mur.
    if(std::fmod(newY, sizeByRoom) <= hitboxSize){
        if(tryFrontier(std::floor(newX/sizeByRoom), std::floor(newY/sizeByRoom), Cell::N)){
            newY = std::floor(newY/sizeByRoom)*sizeByRoom + hitboxSize;
        }
        // Si on est proche de la zone basse de la case, on test si il y a un mur et si il y en a un, alors on recule le nouveau x au niveau de la parois du mur.
    }else if(std::fmod(newY, sizeByRoom) >= sizeByRoom - hitboxSize){
        if(tryFrontier(std::floor(newX/sizeByRoom), std::floor(newY/sizeByRoom), Cell::S)){
            newY = std::floor(newY/sizeByRoom)*sizeByRoom + sizeByRoom - hitboxSize;
        }
    }

    for(unsigned int i = 0; i < collectibles.size(); i++){
        if(abs(newX - collectibles[i]->getX()) < 2 * hitboxSize && abs(newY - collectibles[i]->getY()) < 2 * hitboxSize){
            collectibles[i]->collected();
            // On prévient tous les objets qu'un objet a été collecté. Les objets ayant un lien avec cet objet passe le pointeur vers celui-ci à null
            // De plus, si ils n'ont plus aucun lien avec aucun objet, alors il mette la variable d'autodestruction à true.
            for(unsigned int j = 0; j < collectibles.size(); j++){
                collectibles[j]->thisObjectHasBeenCollected(collectibles[i]);
            }
        }
    }

    // Les objets qui on été collectés ou dont les effets s'appliquent à des objets déjà collectés retourne true à la fonction HasBeenCollected
    for(unsigned int i = 0; i < collectibles.size(); i++){
        if(collectibles[i]->HasBeenCollected()){
            delete collectibles[i];
            collectibles.erase(collectibles.begin() + i);
            i--;
        }
    }

    player.setPosition(newX, newY);
    if(newX < 0 || newX > width_ * sizeByRoom || newY < 0 || newY > height_ * sizeByRoom) // Si on est sorti du labyrinthe
        timer.restart();
}

void Maze::removeWall(){
    switch (rand()%4) {
    case(0):
        grid_[0][rand()%width_].setFrontier(Cell::N, false);
        break;
    case(1):
        grid_[rand()%height_][width_-1].setFrontier(Cell::E, false);
        break;
    case(2):
        grid_[height_-1][rand()%width_].setFrontier(Cell::S, false);
        break;
    case(3):
        grid_[rand()%height_][0].setFrontier(Cell::W, false);
        break;
    }
}



/* Collectibles */
Collectible::Collectible(QString imageName){
    if(imageName != "none"){
        quadrique = gluNewQuadric();
        gluQuadricTexture(quadrique, GL_TRUE);

        QString path = QString(":/tse.png");
        QImage image = QGLWidget::convertToGLFormat(QImage(path));
        glGenTextures(1, &textId);
        glBindTexture(GL_TEXTURE_2D, textId);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }

    totalTime = (rand()%1000)/500.f;
}


Collectible::~Collectible(){
    gluDeleteQuadric(quadrique);
}

void Collectible::display(float elapsedTime){
    totalTime += elapsedTime;
    glPushMatrix();

    float white[4] = {1, 1, 1, 1};
    float black[4] = {0, 0, 0, 1};

    // On dessine une seconde sphère texturée qui ne se voit pas derrière un mur
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textId); // On définit la texture courante

    gluSphere(quadrique, RAYON, 50, 50);
    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);

    glPopMatrix();
}


Key::Key(Maze* m) : Collectible(QString("tse")){
    maze = m;
}

void Key::collected(){
    hasBeenCollected = true;
    maze->removeWall();
}

void Key::display(float elapsedTime){
    totalTime += elapsedTime;
    glPushMatrix();

    glTranslated(posX, z + 0.1 * std::sin(totalTime/300.f), posY);
    glRotatef(360 * std::fmod(totalTime/4000.f, 1), 0, 1, 0);
    glRotatef(-90, 1, 0, 0);

    float white[4] = {1, 1, 1, 1};
    float black[4] = {0, 0, 0, 1};
    if(seeThroughWall){
        /* On dessine une première sphère qui se voit à travers les murs et qui a une couleur orange*/
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glDisable(GL_LIGHTING);
        glColor4f(1, 0.6, 0.2, 0.6);

        gluSphere(quadrique, RAYON, 50, 50);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LIGHTING);
    }

    // On dessine une seconde sphère texturée qui ne se voit pas derrière un mur
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textId); // On définit la texture courante

    gluSphere(quadrique, RAYON, 50, 50);
    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);

    glPopMatrix();
}

Glasses::Glasses(Key* k) : Collectible("none"){
    key = k;
}

void Glasses::display(float elapsedTime){
    totalTime += elapsedTime;
    glPushMatrix();

    // Positionne l'objet à sa position et on lui fait un effet de flottement
    glTranslated(posX, z + 0.1 * std::sin(totalTime/300.f), posY);
    glRotatef(360 * std::fmod(totalTime/4000.f, 1), 0, 1, 0);

    glDisable(GL_LIGHTING);
    float width = 0.6, height = 0.6, glassSize = 0.2;
    glColor3ub(20, 30, 20);
    // On dessine ici les lunettes 3D
    glBegin(GL_QUADS);
        glVertex3f(-width/2, -glassSize/4, -height/2);
        glVertex3f(-width/2, +glassSize/4, -height/2);
        glVertex3f(-width/2, +glassSize/4, height/2);
        glVertex3f(-width/2, -glassSize/4, height/2);

        glVertex3f(width/2, -glassSize/4, -height/2);
        glVertex3f(width/2, +glassSize/4, -height/2);
        glVertex3f(width/2, +glassSize/4, height/2);
        glVertex3f(width/2, -glassSize/4, height/2);

        glColor3ub(50, 30, 180);
        glVertex3f(width/2 - 4*width/10, +glassSize/8, height/2);
        glVertex3f(width/2 - 4*width/10, -glassSize/8, height/2);
        glVertex3f(width/2 - 6*width/10, -glassSize/8, height/2);
        glVertex3f(width/2 - 6*width/10, +glassSize/8, height/2);
    glEnd();
    glBegin(GL_QUAD_STRIP);
        glVertex3f(width/2, +glassSize/2, height/2);
        glVertex3f(width/2-width/10, glassSize/2-glassSize/6, height/2);
        glVertex3f(width/2, -glassSize/2, height/2);
        glVertex3f(width/2-width/10, -glassSize/2+glassSize/6, height/2);

        glVertex3f(width/2-4*width/10, -glassSize/2, height/2);
        glVertex3f(width/2-3*width/10, -glassSize/2+glassSize/6, height/2);

        glVertex3f(width/2-4*width/10, +glassSize/2, height/2);
        glVertex3f(width/2-3*width/10, +glassSize/2-glassSize/6, height/2);

        glVertex3f(width/2, +glassSize/2, height/2);
        glVertex3f(width/2-width/10, glassSize/2-glassSize/6, height/2);
    glEnd();
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-width/2, +glassSize/2, height/2);
        glVertex3f(-width/2+width/10, glassSize/2-glassSize/6, height/2);
        glVertex3f(-width/2, -glassSize/2, height/2);
        glVertex3f(-width/2+width/10, -glassSize/2+glassSize/6, height/2);

        glVertex3f(-width/2+4*width/10, -glassSize/2, height/2);
        glVertex3f(-width/2+3*width/10, -glassSize/2+glassSize/6, height/2);

        glVertex3f(-width/2+4*width/10, +glassSize/2, height/2);
        glVertex3f(-width/2+3*width/10, +glassSize/2-glassSize/6, height/2);

        glVertex3f(-width/2, +glassSize/2, height/2);
        glVertex3f(-width/2+width/10, glassSize/2-glassSize/6, height/2);
    glEnd();
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void Glasses::collected(){
    hasBeenCollected = true;
    if(key != nullptr) // Si la clé n'a pas encore été ramassé
        key->seeThrough(); // On active le fait de voir la clé à travers les murs
}

void Glasses::thisObjectHasBeenCollected(Collectible * c){
    if(c == key){ // Si l'objet rammassé correspond à la clé, alors on doit détruire les lunettes car elles ne servent plus à rien.
        key = nullptr;
        destroyIt();
    }
}

QString intToStringHourFormat(long int v){
    QString time;
    if(v < 10)
        time.append("0");
    time.append(QString::number(v));
    return time;
}
