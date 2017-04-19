#include "gamemanager.h"

#include <QPainter>

GameManager::GameManager(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(900, 700);
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
    testAlien = new Aliens(0, 57, 40);
    testAlien2 = new Aliens(1, 107, 40);
    testAlien3 = new Aliens(2, 157, 40);

    alienAnimationTimer = new QTimer(this);
    alienAnimationTimer->setInterval(500);
    connect(alienAnimationTimer, SIGNAL(timeout()), this, SLOT(update()));
    alienAnimationTimer->start();

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
//            paint.drawText(posX + 15, posY + 25, QString("%1, %2").arg(i).arg(j));
            paint.drawLine(posX, posY, posX, posY + 50);
            posX += 50;
        }

        posX = 50;
        posY += 50;
    }

    paint.drawLine(850, 30, 850, posY);
    paint.drawLine(posX, posY, 850, posY);

    //=================
    // Enemy Rendering
    //=================
    paint.setBrush(QBrush(Qt::white));
    testAlien->drawAlien(&paint);
    testAlien2->drawAlien(&paint);
    testAlien3->drawAlien(&paint);

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

    player->drawPlayer(&paint);


    //================
    // UI Rendering
    //================

    paint.drawLine(QLine(0, 678, 900, 678));
    QFont font("TypeWriter", 10, 1);
    paint.setFont(font);
    paint.drawText(20, 690, QString("%1").arg(player->GetLivesRemaining()));
    paint.drawText(20, 20, "<<ADD SCORE TRACKER>>");
    paint.drawText(380, 20, "<<ADD HIGH SCORE>>");
    /// Add score tracker
}
