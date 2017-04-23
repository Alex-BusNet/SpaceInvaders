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
    this->setFixedSize(900, 750);

    redrawAliens = true;
    redrawBunkers = true;
    shiftAliens = false;
    left = false;
    pauseGame = false;
    invadersTopRow = 0;
    invadersLeftColumn = 0;
    player = new Player(428, 680);

    // Initialize the Aliens
    int alienX = 32, alienY = 80;
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

    bulletUpdateTimer = new QTimer(this);
    bulletUpdateTimer->setInterval(15);
    connect(bulletUpdateTimer, SIGNAL(timeout()), this, SLOT(updateBullets()));

    alienAnimationTimer->start();
    gameUpdateTimer->start();
    bulletUpdateTimer->start();
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

    int posX = 30, posY = 70;
    paint.drawLine(posX, 30, GRID_END, 30);
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
    paint.drawLine(this->width() / 4, 590, this->width() / 4, 750);
    paint.drawLine(this->width() / 2, 590, this->width() / 2, 750);
    paint.drawLine((this->width() / 2) + (this->width() / 4), 590, (this->width() / 2) + (this->width() / 4), 750);

    //======================
    // Projectile Rendering
    //======================


    for(int i = 0; i < 5; i++){
        if(bullets[i] != NULL)
            bullets[i]->drawBullet(&paint);
    }
    /// DO NOT write collision logic here
    /// Handle collision logic in a seperate function
    ///  that is controlled by a seperate timer.
    ///
    /// Projectile size: 1px X 3px
    ///     You could make them bigger just adjust how many pixels are removed in the i direction
    ///     Use (i-1) and check for (i-1) >= 0
    ///
    /// To determine if a bunker was hit, which bunker was hit, and where, use:
    ///     for(int h = 0; h < 5; h++)
    ///     {
    ///         if(bullets[h] != NULL)
    ///         {
    ///             if(bullets[h]->GetPosY() > 580)
    ///             {
    ///                 bunker_k = bullets[h]->GetPosX() / 225;
    ///                 bunker_j = bullets[h]->GetPosX() - ((75 * (bunker_k + 1) + (220 * bunker_k));
    ///                 bunker_i = 550 - bullets[h]->GetPosY();
    ///                 if(bunker_i >= 0 && bunker_j >= 0 && bunker_j < 44 && bunker_k >= 0 && bunker_k < 4)
    ///                 {
    ///                     if(bunkers[bunker_k][bunker_i][bunker_j] == 1)
    ///                     {
    ///                         section_i = bunker_i % 11;
    ///                         section_j = bunker_j % 5;
    ///                         for(int i = (section_i * 6); i < ((section_i + 1) *6 ); i++)
    ///                         {
    ///                             for(int j = (section_j * 4); j < ((section_j + 1) * 4); j++)
    ///                             {
    ///                                 bunkers[bunker_k][i][j] = 0;
    ///                             }
    ///                         }
    ///                 }
    ///             }
    ///         }
    ///     }
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
    ///     30 = X grid offset
    ///     80 = Y grid offset
    ///
    ///     Find grid coord to determine if an alien is present:
    ///         grid_j = (bulletPosX - 30) / 40;
    ///         grid_i = (bulletPosY - 80) / 40;
    ///
    ///     Find invader coord to determine type:
    ///         invader_i = grid_i - invadersTopRow;
    ///         invader_j = grid_j - invadersLeftColumn;
    ///
    ///     Find bullet position relative to the alien:
    ///         bulletRelPosX = grid_i % 40;
    ///         bulletRelPosY = grid_j % 40;
    ///             if(bulletRelPosX >= 2 && bulletRelPosX <= 38)
    ///             {
    ///                 bulletRelPosX = (bulletRelPosX - 2) / 3;
    ///
    ///                 if(bulletRelPosY >= 10 && bulletRelPosY <= 34)
    ///                 {
    ///                     bulletRelPosY = (bulletRelPosY - 10) / 3;
    ///                     a->CheckCollision(bulletRelPosX, bulletRelPosY, invaders[invader_i][invader_j]);
    ///                 }
    ///             }
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
                    else
                    {
                        grid[i + 1][j] = grid[i][j];
                        grid[i][j] = 0;
                    }
                }
            }
        }

        invadersTopRow++;
    }

    foreach(Alien *a, alienVec)
    {
        a->drawAlien(&paint, redrawAliens, shiftAliens);
    }


    if(redrawAliens && shiftAliens)
    {
        shiftAliens = false;
    }

    if(redrawAliens)
    {
        if(left)
        {
            // Count how many consecutive zeros are in the each column
            int count[COLUMNS] = {};
            for(int i = 0; i < ROWS; i++)
            {
                for(int j = 0; j < COLUMNS; j++)
                {
                    if(grid[i][j] == 1)
                    {
                        // Reset the count.
                        count[j] = 0;
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
                    else
                    {
                        if(count[j] == ROWS)
                            invadersLeftColumn = j + 1;
                        else
                            count[j] = count[j] + 1;
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
            // Count how many consecutive zeros are in the each column
            int count[COLUMNS] = {};
            for(int i = ROWS - 1; i >= 0; i--)
            {
                for(int j = COLUMNS - 1; j >= 0; j--)
                {
                    if(grid[i][j] == 1)
                    {
                        //Reset the count
                        count[j] = 0;
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
                    else
                    {
                        if((count[j] == ROWS) && (j == (invadersLeftColumn + 1)))
                            invadersLeftColumn++;
                        else
                            count[j] = count[j] + 1;
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
    int startX = 75, bunkerX = 75, bunkerY = 590;
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

        bunkerY = 590;
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

    paint.drawLine(QLine(0, 720, 900, 720));
    QFont font("TypeWriter", 10, 1);
    paint.setFont(font);
    paint.drawText(20, 735, QString("%1").arg(player->GetLivesRemaining()));
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

void GameManager::addBullet(bool player, int posX, int posY)
{
    if(player)
    {
        bullets[0] = new Bullet(true, posX, posY);
    }
    else
    {
        for(int i = 1; i < 5; i++)
        {
            if(bullets[i] == NULL)
            {
                bullets[i] = new Bullet(false, posX, posY);
                break;
            }
        }
    }
}

void GameManager::updateBullets()
{
    for(int i = 0; i < 5; i++)
        if(bullets[i] != NULL)
        {
            bullets[i]->UpdatePos();

            /// Add collision checks here.

            if(bullets[i]->posY <= 70)
            {
                bullets[i] = NULL;
                if(i == 0)
                {
                    player->ResetLaser();
                }
                else
                {
                    alienVec.at(i - 1)->ResetFire();
                }
            }
        }
}

void GameManager::updateAliens()
{
    redrawAliens = true;
}

/// alienFire Logic
///
/// invader_i = rand() % 40;
/// invader_j = rand() % 40;
///     if(grid[i][j] == 1)
///     {
///         invader_i = grid_i - invadersTopRow;
///         invader_j = grid_j - invadersLeftColumn;
///         alienVec.at(invader_i + invader_j)->Fire();
///     }
///
