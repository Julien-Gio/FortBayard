#include "menuwindow.h"

#include <QApplication>


char MenuWindow::diff = 'N';

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MenuWindow w;
    w.show();
    return a.exec();
}
