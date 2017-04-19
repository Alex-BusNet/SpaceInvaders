#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamemanager.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_newGamePB_clicked();

private:
    Ui::MainWindow *ui;
    GameManager *gm;
};

#endif // MAINWINDOW_H
