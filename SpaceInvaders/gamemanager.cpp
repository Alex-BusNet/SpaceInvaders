#include "gamemanager.h"
#include <QDebug>
#include <QPainter>

GameManager::GameManager(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(900, 700);

    redrawAliens = true;
    redrawPlayer = true;
    redrawBunkers = true;

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

    qDebug() << "Bunker grid cell size:" << ((this->width() / 2) - (this->width() / 4));
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
            // Grid Coords
//            paint.drawText(posX + 15, posY + 25, QString("%1, %2").arg(i).arg(j));

            // Main Grid
            paint.drawLine(posX, posY, posX, posY + 50);
            posX += 50;
        }

        posX = 50;
        posY += 50;
    }

    // These two lines make the grid look more complete
    paint.drawLine(850, 30, 850, posY);
    paint.drawLine(posX, posY, 850, posY);

    // Bunker Location Grid
    paint.drawLine(this->width() / 4, 540, this->width() / 4, 700);
    paint.drawLine(this->width() / 2, 540, this->width() / 2, 700);
    paint.drawLine((this->width() / 2) + (this->width() / 4), 540, (this->width() / 2) + (this->width() / 4), 700);

    //======================
    // Projectile Rendering
    //======================

    /// DO NOT write collision logic here
    /// Handle collision logic in a seperate function
    ///  that is controlled by a seperate timer.
    ///
    /// Projectile size: 1px X 3px OR 2px X 5px (Pick one, I don't care which.)
    ///     You could make them bigger just adjust how many pixels are removed in the i direction
    ///     Use (i-1) and check for (i-1) >= 0
    ///
    /// To determine if a bunker was hit, which bunker was hit, and where, use:
    ///    k = projectilePosX / 225
    ///    j = projectilePosX - ((75 * (k + 1) + (220 * k))
    ///    i = 550 - projectilePosY
    ///
    ///    Be sure to add checks for k, j, and i >= 0 AND j < 44 AND i < 30 AND k < 4
    ///     before accessing the arrays.
    ///
    ///     Bunkers are 88px X 60px
    ///       Bunker 0 domain: 75px - 163px
    ///       Bunker 1 domain: 295px - 383px
    ///       Bunker 2 domain: 515px - 603px
    ///       Bunker 3 domain: 735px - 823px
    ///     Bunker range: 550px - 610px
    ///
    /// To determine if the player was hit, use:
    ///     << WIP >>
    ///
    ///
    /// To determine if an invader was hit, use:
    ///     << WIP >>
    ///
    ///     Invaders are 24px X 36px
    ///

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
    int startX = 75, bunkerX = 75, bunkerY = 550;
    for(int k = 0; k < 4; k++)
    {
        for(int i = 0; i < 30; i++)
        {
            for(int j = 0; j < 44; j++)
            {
                if(bunker[k][i][j] == 1)
                    paint.drawRect(QRect(bunkerX, bunkerY, 2, 2));

                bunkerX += 2;
            }

            bunkerY += 2;
            bunkerX = startX;
        }
        bunkerY = 550;
        startX = 75 + (220 * (k + 1));
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
