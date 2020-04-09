#include "menuwindow.h"
#include "ui_menuwindow.h"
#include "mainwindow.h"
#include <iostream>

MenuWindow::MenuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuWindow) {
    ui->setupUi(this);

    defaultFont = ui->EasyBtn->font();
    bigFont = ui->NormalBtn->font();
    MenuWindow::diff = 'N';
}

MenuWindow::~MenuWindow() {
    delete ui;
}

void MenuWindow::on_StartBtn_clicked() {
    MainWindow* mw = new MainWindow(camera);
    mw->show();
    close();
}


void MenuWindow::on_EasyBtn_clicked() {
    MenuWindow::diff = 'E';
    ui->EasyBtn->setFont(bigFont);
    ui->NormalBtn->setFont(defaultFont);
    ui->HardBtn->setFont(defaultFont);
}


void MenuWindow::on_NormalBtn_clicked() {
    MenuWindow::diff = 'N';
    ui->EasyBtn->setFont(defaultFont);
    ui->NormalBtn->setFont(bigFont);
    ui->HardBtn->setFont(defaultFont);
}


void MenuWindow::on_HardBtn_clicked() {
    MenuWindow::diff = 'H';
    ui->EasyBtn->setFont(defaultFont);
    ui->NormalBtn->setFont(defaultFont);
    ui->HardBtn->setFont(bigFont);
}

void MenuWindow::on_KeyboardBtn_clicked()
{
    camera = false;
    ui->KeyboardBtn->setFont(bigFont);
    ui->CameraBtn->setFont(defaultFont);
}

void MenuWindow::on_CameraBtn_clicked()
{
   camera = true;
    ui->KeyboardBtn->setFont(defaultFont);
    ui->CameraBtn->setFont(bigFont);
}
