#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menuwindow.h"
#include <QCloseEvent>

MainWindow::MainWindow(bool playWithCamera, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->openGLWidget->init(playWithCamera);
    ui->openGLWidget->grabKeyboard();
    ui->openGLWidget->setMainWindow(this);

    connect(ui->openGLWidget, SIGNAL(goToMainMenu()), this, SLOT(goToMainMenu()));
}


MainWindow::~MainWindow()
{
    delete ui;
    std::cout << "deleteMain" << std::endl;

}


void MainWindow::closeEvent(QCloseEvent *event) {
    std::cout << "mainclose"<< std::endl;
    event->accept();
    emit openMenu();
}


void MainWindow::setImageLbl(QImage& img) {
    // Display on label
    ui->camera->setPixmap(QPixmap::fromImage(img));
    // Resize the label to fit the image
    ui->camera->resize(ui->camera->pixmap()->size());
}


// Slots
void MainWindow::goToMainMenu() {
    std::cout << "goToMainMenu"<< std::endl;
    close();
}
