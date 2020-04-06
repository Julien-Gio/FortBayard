#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->openGLWidget->grabKeyboard();
    ui->openGLWidget->setMainWindow(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setImageLbl(QImage& img) {
    // Display on label
    ui->camera->setPixmap(QPixmap::fromImage(img));
    // Resize the label to fit the image
    ui->camera->resize(ui->camera->pixmap()->size());
}
