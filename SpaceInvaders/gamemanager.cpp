#include "gamemanager.h"
#include <QDebug>
#include <QPainter>

GameManager::GameManager(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(900, 700);

    redrawAliens = true;
    redrawPlayer = true;
    redrawBunkers = true;

    int bunkerX = 100, bunkerY = 550;
    for(int k = 0; k < 4; k++)
    {

        bunkers.push_back(
                    new QPolygon(
                            QVector<QPoint>
                            {
                                QPoint(bunkerX + 20, bunkerY),
                                QPoint(bunkerX + 70, bunkerY),
                                QPoint(bunkerX + 90, bunkerY + 20),
                                QPoint(bunkerX + 90, bunkerY + 60),
                                QPoint(bunkerX + 70, bunkerY + 60),
                                QPoint(bunkerX + 60, bunkerY + 45),
                                QPoint(bunkerX + 30, bunkerY + 45),
                                QPoint(bunkerX + 20, bunkerY + 60),
                                QPoint(bunkerX, bunkerY + 60),
                                QPoint(bunkerX, bunkerY + 20)
                            }
                        )
                    );

        bunkerX += 200;
    }

    player = new Player(428, 630);

    // Initialize the Aliens
    int alienX = 57, alienY = 40;
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 11; j++)
        {
            alienVec.push_back(new Alien(invaders[i][j], alienX, alienY));
            alienX += 50;

        }

        alienX = 57;
        alienY += 50;
    }

    alienAnimationTimer = new QTimer(this);
    alienAnimationTimer->setInterval(500);
    connect(alienAnimationTimer, SIGNAL(timeout()), this, SLOT(updateAliens()));

    gameUpdateTimer = new QTimer(this);
    gameUpdateTimer->setInterval(17);
    connect(gameUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));

    alienAnimationTimer->start();
    gameUpdateTimer->start();

}

void GameManager::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);

    paint.setPen(Qt::white);
    paint.setBrush(QBrush(Qt::black));
    paint.drawRect(this->rect());

    //=================
    // Grid Rendering;
    //=================

    int posX = 50, posY = 30;
    for(int i = 0; i < 10; i++)
    {
        paint.drawLine(posX, posY, 850, posY);
        for(int j = 0; j < 16; j++)
        {
            //Grid Coords
//            paint.drawText(posX + 15, posY + 25, QString("%1, %2").arg(i).arg(j));

            paint.drawLine(posX, posY, posX, posY + 50);
            posX += 50;
        }

        posX = 50;
        posY += 50;
    }

    paint.drawLine(850, 30, 850, posY);
    paint.drawLine(posX, posY, 850, posY);

    //======================
    // Projectile Rendering
    //======================

    /// DO NOT write collision logic here
    /// Handle collision logic in a seperate function
    ///  that is controlled by a seperate timer.

    //=================
    // Enemy Rendering
    //=================

    paint.setBrush(QBrush(Qt::white));
    // Some of this render logic may changed depending on
    //   what happens with tracking living invaders
    foreach(Alien *a, alienVec)
    {
        //if(redrawAliens)
        //{
        /// Add Update Position logic before redraw
        //}

        a->drawAlien(&paint, redrawAliens);
    }

    redrawAliens = false;

    //==================
    // Bunker Rendering
    //==================

    paint.setBrush(QBrush(Qt::green));
    paint.setPen(QPen(Qt::green));
    foreach(QPolygon* r, bunkers)
    {
        paint.drawPolygon(*r);
    }

    //==================
    // Player Rendering
    //==================

    player->UpdatePosition();
    player->drawPlayer(&paint);

    //================
    // UI Rendering
    //================

    paint.drawLine(QLine(0, 670, 900, 670));
    QFont font("TypeWriter", 10, 1);
    paint.setFont(font);
    paint.drawText(20, 690, QString("%1").arg(player->GetLivesRemaining()));
    paint.drawText(20, 20, "<<ADD SCORE TRACKER>>");
    paint.drawText(380, 20, "<<ADD HIGH SCORE>>");
    /// Add score tracker
}

void GameManager::keyPressEvent(QKeyEvent *event)
{
    player->InputHandler(static_cast<QKeyEvent*>(event), true);
    event->accept();
}

void GameManager::keyReleaseEvent(QKeyEvent *event)
{
    player->InputHandler(static_cast<QKeyEvent*>(event), false);
    event->accept();
}

void GameManager::updateAliens()
{
    redrawAliens = true;
}
