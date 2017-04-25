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
    player = new Player(422, 680);

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
//    paint.drawLine(posX, 590, GRID_END, 590);
    paint.drawLine(posX, 670, GRID_END, 670);

    // Bunker Location Grid
    paint.drawLine(30, 590, 30, 720);
    paint.drawLine(240, 590, 240, 720);
    paint.drawLine(450, 590, 450, 720);
    paint.drawLine(650, 590, 650, 720);
    paint.drawLine(870, 590, 870, 720);

    //======================
    // Projectile Rendering
    //======================

    for(int i = 0; i < 5; i++)
    {
        if(bullets[i] != NULL)
            bullets[i]->drawBullet(&paint);
    }

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
    int startX = 91, bunkerX = 91, bunkerY = 590;
    for(int k = 0; k < 4; k++)
    {
        for(int i = 0; i < 30; i++)
        {
            for(int j = 0; j < 44; j++)
            {
                if(bunker[k][i][j] == 1)
                    paint.drawRect(QRect(bunkerX, bunkerY, 1, 1));

                bunkerX += 2;
            }

            bunkerY +=2;
            bunkerX = startX;
        }

        bunkerY = 590;
        startX = 91 + (210 * (k + 1));
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
                bulletIndex = i;
                break;
            }
        }
    }
}

void GameManager::updateBullets()
{
    bool deleteBullet = false;
    for(int h = 0; h < 5; h++)
    {
        if(bullets[h] != NULL)
        {
            bullets[h]->UpdatePos();

            //=================
            // Bunker Collision
            //=================
            if(bullets[h]->GetPosY() >= 590 && bullets[h]->GetPosY() <= 650)
            {
                // Bunker Coordinates calculations
                int bunker_k = bullets[h]->GetPosX() / 225;
                int bunker_j = (bullets[h]->GetPosX() - (91 + (210 * bunker_k))) / 2;
                int bunker_i = ((bullets[h]->GetPosY() - 590) / 2) - 1;

                if(bunker_i >= 0 && bunker_i <= 29 && bunker_j >= 0 && bunker_j < 44 && bunker_k >= 0 && bunker_k < 4)
                {
                    // Check if the location in the bunker is valid
                    if(bunker[bunker_k][bunker_i][bunker_j] == 1)
                    {
                        deleteBullet = true;
                        int section_i = bunker_i / 5;
                        int section_j = bunker_j / 4;
                        for(int i = (section_i * 5); i < ((section_i + 1) * 5); i++)
                        {
                            for(int j = (section_j * 4); j < ((section_j + 1) * 4); j++)
                            {
                                bunker[bunker_k][i][j] = 0;
                            }
                        }
                    }
                }
            }
            else
            //=================
            // Alien Collision
            //=================
            {
                qDebug() << "Checking alien collisions";
                int grid_j = (bullets[h]->GetPosX() - 30) / 40;
                int grid_i = (bullets[h]->GetPosY() - 80) / 40;

                // Find invader coord to determine type:
                int invader_i = grid_i - invadersTopRow;
                int invader_j = grid_j - invadersLeftColumn;

                if(grid[invader_i][invader_j] == 1)
                {
                    // Find bullet position relative to the grid cell:
                    int bulletRelPosX = grid_i % 40;
                    int bulletRelPosY = grid_j % 40;

                    if(bulletRelPosX >= 2 && bulletRelPosX <= 38)
                    {
                        if(bulletRelPosY >= 10 && bulletRelPosY <= 34)
                        {
                            //Calculate bullet position relative to alien
                            bulletRelPosX = (bulletRelPosX - 2) / 3;
                            bulletRelPosY = (bulletRelPosY - 10) / 3;
                            if(alienVec.at(invader_i + invader_j)->CheckCollision(bulletRelPosX, bulletRelPosY, invaders[invader_i][invader_j]))
                            {
                                grid[grid_i][grid_j] = 0;
                                deleteBullet = true;
                            }
                        }
                    }
                }
            }

            /// To determine if the player was hit, use:
            ///     << WIP >>
            ///
            ///

            // Bullet has reached edge of play area
            if(bullets[h]->GetPosY() <= 30 || deleteBullet)
            {
                deleteBullet = false;
                bullets[h] = NULL;

                if(h == 0)
                {
                    player->ResetLaser();
                }
                else
                {
                    qDebug() << "reset alien fire flag";
                    alienVec.at(alienFireTracker[h]);
                    alienFireTracker[h] = -1;
                }
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
///     grid_i = rand() % 40;
///     grid_j = rand() % 40;
///     if(grid[i][j] == 1)
///     {
///         invader_i = grid_i - invadersTopRow;
///         invader_j = grid_j - invadersLeftColumn;
///         alienVec.at(invader_i + invader_j)->Fire();
///         alienFireTracker[bulletIndex] = (invader_i + invader_j);
///     }
///
