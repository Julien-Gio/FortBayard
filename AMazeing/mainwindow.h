#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>

/*
 * Classe de la fenetre de jeu.
 * S'occupe d'afficher le labyrinthe 3D avec un widget openGL.
 **/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool playWithCamera, QWidget *parent = nullptr);
    ~MainWindow();

    void setImageLbl(QImage& img);

public slots:
    void goToMainMenu();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
