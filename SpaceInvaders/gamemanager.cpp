#include "gamemanager.h"
#include <QDebug>
#include <QPainter>
#define ROWS 15
#define COLUMNS 30
#define X_OFFSET 40
#define Y_OFFSET 40
#define SHIFT_AMOUNT 20
#define GRID_END ((X_OFFSET * 11) + (SHIFT_AMOUNT * (COLUMNS - 9)) + 10)

GameManager::GameManager(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(900, 700);

    redrawAliens = true;
    redrawBunkers = true;
    shiftAliens = false;
    left = false;
    pauseGame = false;

    player = new Player(428, 630);

    // Initialize the Aliens
    int alienX = 32, alienY = 40;
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 11; j++)
        {
            alienVec.push_back(new Alien(invaders[i][j], alienX, alienY));
            grid[i][j] = 1;
            alienX += X_OFFSET;
        }

        alienX = 32;
        alienY += Y_OFFSET;
    }

    alienAnimationTimer = new QTimer(this);
    alienAnimationTimer->setInterval(500);
    connect(alienAnimationTimer, SIGNAL(timeout()), this, SLOT(updateAliens()));

    gameUpdateTimer = new QTimer(this);
    gameUpdateTimer->setInterval(17);
    connect(gameUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));

    alienAnimationTimer->start();
    gameUpdateTimer->start();

    qDebug() << "Grid end:" << GRID_END;
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

    int posX = 30, posY = 30;
    for(int i = 0; i < ROWS; i++)
    {
        paint.drawLine(posX, posY, GRID_END, posY);
        for(int j = 0; j < COLUMNS; j++)
        {
            // Grid Coords
//            paint.drawText(posX + 1, posY + 15, QString("%1").arg(i));
//            paint.drawText(posX + 1, posY + 25, QString("%1").arg(j));
//            paint.drawText(posX + 15, posY + 35, QString("%1").arg(grid[i][j]));

            // Main Grid
            paint.drawLine(posX, posY, posX, posY + Y_OFFSET);
            posX += X_OFFSET;
        }

        posX = 30;
        posY += Y_OFFSET;
    }

    // These two lines make the grid look more complete
    paint.drawLine(GRID_END, 30, GRID_END, posY);
    paint.drawLine(posX, posY, GRID_END, posY);

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
    ///     Invaders are 36px X 24px
    ///

    //=================
    // Enemy Rendering
    //=================

    paint.setBrush(QBrush(Qt::white));
    // Some of this render logic may changed depending on
    //   what happens with tracking living invaders

    if(shiftAliens)
    {
        for(int i = ROWS - 1; i >= 0; i--)
        {
            for(int j = COLUMNS - 1; j >= 0; j--)
            {
                if(grid[i][j] == 1)
                {
                    if((i + 1) >= ROWS)
                    {
                        // Game Over
                    }

                    grid[i + 1][j] = grid[i][j];
                    grid[i][j] = 0;
                }
            }
        }
    }

    foreach(Alien *a, alienVec)
    {
        a->drawAlien(&paint, redrawAliens);

        if(shiftAliens)
        {
            a->shiftDown();
        }
    }

    shiftAliens = false;

    if(redrawAliens)
    {
        if(left)
        {
            for(int i = 0; i < ROWS; i++)
            {
                for(int j = 0; j < COLUMNS; j++)
                {
                    if(grid[i][j] == 1)
                    {
                        if(j == 0)
                        {
                            left = false;
                            shiftAliens = true;
                            break;
                        }
                        else
                        {
                            grid[i][j - 1] = grid[i][j];
                            grid[i][j] = 0;
                        }
                    }
                }

                if(shiftAliens)
                {
                    break;
                }
            }

        }
        else
        {
            for(int i = ROWS - 1; i >= 0; i--)
            {
                for(int j = COLUMNS - 1; j >= 0; j--)
                {
                    if(grid[i][j] == 1)
                    {
                        if(j == (COLUMNS - 1))
                        {
                            left = true;
                            shiftAliens = true;
                            break;
                        }
                        else
                        {
                            if(j + 1 < COLUMNS)
                            {
                                grid[i][j + 1] = grid[i][j];
                                grid[i][j] = 0;
                            }
                        }
                    }
                }

                if(shiftAliens)
                {
                    break;
                }
            }
        }
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
    player->InputHandler(event, true);
    event->accept();
}

void GameManager::keyReleaseEvent(QKeyEvent *event)
{
    player->InputHandler(event, false);
    event->accept();
}

void GameManager::updateAliens()
{
    redrawAliens = true;
}
