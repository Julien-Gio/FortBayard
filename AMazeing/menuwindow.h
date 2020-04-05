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

private slots:
    /* Rôle : lance une partie */
    void on_StartBtn_clicked();

    /* Rôle : passe en difficulté normale */
    void on_NormalBtn_clicked();

    /* Rôle : passe en difficulté facile */
    void on_EasyBtn_clicked();

    /* Rôle : passe en difficulté difficile */
    void on_HardBtn_clicked();

private:
    Ui::MenuWindow *ui;

    // Polices utilisés par les emojis pour la difficulté
    QFont defaultFont;
    QFont bigFont;
};

#endif // MENUWINDOW_H
