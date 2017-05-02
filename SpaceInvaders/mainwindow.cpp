#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QShortcut>
#include <qjsondocument.h>


QKeySequence *resetScores = new QKeySequence("Ctrl+R");
QShortcut *rsShortcut;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString mainStyle = "QWidget { background-color: black; } QFrame { background-color: lime; }";
    mainStyle += "QPushButton { background-color: black; color: lime; border: 1px solid lime; } ";
    mainStyle += "QPushButton:pressed { background-color: grey; }";
    mainStyle += "QListWidget { background-color: black; color: lime; font: normal TypeWriter 24px; border: 1px inset lime; }";
    mainStyle += "QListWidget::item { min-height: 27px; alignment: center; }";
    mainStyle += "QLabel { background-color: black; color: lime; }";

    ui->setupUi(this);
    gm = NULL;
    updateTimer = new QTimer();
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateHighScores()));

    rsShortcut = new QShortcut(*resetScores, this);
    connect(rsShortcut, SIGNAL(activated()), this, SLOT(resetHighScores()));

    this->setFixedSize(this->width(),this->height());

    this->setStyleSheet(mainStyle);
    this->setWindowTitle("Space Invaders");

    loadHighScores();

    ui->listWidget->setSelectionMode(QListWidget::NoSelection);
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

    updateTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newGamePB_clicked()
{
    gm = new GameManager(NULL, ui->listWidget->item(0)->text().split('.').at(1).trimmed().toInt());
    gm->show();
}

void MainWindow::on_CloseGamePB_clicked()
{
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

void MainWindow::updateHighScores()
{
//    if(gm != NULL && gm->isHidden())
//    {
        QFile highscores("../SpaceInvaders/Assets/highscores.json");
        if(!highscores.open(QIODevice::ReadWrite))
        {
            qWarning("Could not open highscores file");
            return;
        }
        QByteArray byteArr = highscores.readAll();
        QJsonDocument hs = QJsonDocument::fromJson(byteArr);
        QJsonObject obj = hs.object();
        QJsonArray scores = obj["scores"].toArray();
        ui->listWidget->clear();

        for(int i = 0; i < 10; i++)
        {
            ui->listWidget->addItem( QString("%1. %2").arg(i + 1).arg(scores.at(i).toInt()));
        }

        highscores.flush();
        highscores.close();
        //    }
}

void MainWindow::resetHighScores()
{
    qDebug() << "Reset High Scores Triggered";
    QFile highscores("../SpaceInvaders/Assets/highscores.json");
    QJsonDocument hs;
    QJsonObject obj;
    QJsonArray scores;

    for(int i = 0; i < 10; i++)
    {
        scores.push_back(0);
    }

    obj["scores"] = scores;
    hs.setObject(obj);
    if(highscores.open(QIODevice::WriteOnly))
    {
        highscores.write(hs.toJson());
    }

    highscores.flush();
    highscores.close();
}

void MainWindow::loadHighScores()
{
    QFile highscores("../SpaceInvaders/Assets/highscores.json");
    if(!highscores.open(QIODevice::ReadWrite))
    {
        qWarning("Could not open highscores file");
        return;
    }
    QByteArray byteArr = highscores.readAll();
    QJsonDocument hs = QJsonDocument::fromJson(byteArr);
    QJsonObject obj = hs.object();
    QJsonArray scores = obj["scores"].toArray();

    for(int i = 0; i < 10; i++)
    {
        ui->listWidget->addItem( QString("%1. %2").arg(i + 1).arg(scores.at(i).toInt()));
    }

    highscores.flush();
    highscores.close();
}
