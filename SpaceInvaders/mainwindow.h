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

    void on_CloseGamePB_clicked();

    void on_ShowControlsPB_clicked();

    void on_ControlClosePB_clicked();

    void updateHighScores();
    void resetHighScores();
private:
    Ui::MainWindow *ui;
    GameManager *gm;

    QTimer *updateTimer;
    void loadHighScores();

};

#endif // MAINWINDOW_H
