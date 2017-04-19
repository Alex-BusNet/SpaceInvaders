#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gm = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newGamePB_clicked()
{
    if(gm != NULL)
    {
        delete gm;
    }

    gm = new GameManager();
    gm->show();
}
