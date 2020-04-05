#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>

namespace Ui {
class MenuWindow;
}

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    static char diff;  // 'E' for easy ; 'N' for normal ; 'H' for hard

    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

    void openMenu();  // Call this when you want to go back to the main menu

private slots:
    void on_StartBtn_clicked();

    void on_NormalBtn_clicked();

    void on_EasyBtn_clicked();

    void on_HardBtn_clicked();

private:
    Ui::MenuWindow *ui;


    QFont defaultFont;
    QFont bigFont;
};

#endif // MENUWINDOW_H
