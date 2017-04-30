#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString mainStyle = "QWidget { background-color: black; } QFrame { background-color: green; }";
    mainStyle += "QPushButton { background-color: black; color: green; border: 1px solid green; } ";
    mainStyle += "QPushButton:pressed { background-color: grey; }";
    mainStyle += "QListWidget { background-color: black; color: green; font: Courier; border: 1px inset green; }";
    mainStyle += "QLabel { background-color: black; color: green; }";

    ui->setupUi(this);
    gm = NULL;

    this->setStyleSheet(mainStyle);

    ui->ControlClosePB->hide();
    ui->frame->hide();
    ui->frame_2->hide();
    ui->frame_3->hide();
    ui->frame_4->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
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

void MainWindow::on_CloseGamePB_clicked()
{
    if(gm != NULL)
    {
        delete gm;
    }

    this->close();
}

void MainWindow::on_ShowControlsPB_clicked()
{
    ui->listWidget->hide();
    ui->ControlClosePB->show();
    ui->frame->show();
    ui->frame_2->show();
    ui->frame_3->show();
    ui->frame_4->show();
    ui->label->show();
    ui->label_2->show();
    ui->label_3->show();
    ui->label_4->show();
    ui->label_5->show();
    ui->label_6->show();
    ui->label_7->show();
    ui->label_8->show();
}

void MainWindow::on_ControlClosePB_clicked()
{
    ui->ControlClosePB->hide();
    ui->listWidget->show();
    ui->frame->hide();
    ui->frame_2->hide();
    ui->frame_3->hide();
    ui->frame_4->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
}
