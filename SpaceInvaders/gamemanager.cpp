#include "gamemanager.h"
#include <QDebug>
#include <QPainter>
#include <random>
#include <ctime>
#define ROWS 15
#define COLUMNS 30
#define X_OFFSET 30
#define Y_OFFSET 40
#define SHIFT_AMOUNT 30
#define GRID_END ((X_OFFSET * 11) + (SHIFT_AMOUNT * (COLUMNS - 11)) + 15)

GameManager::GameManager(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(930, 750);

    SetupGame(true);
    srand(time(0));

    alienAnimationTimer = new QTimer(this);
    alienAnimationTimer->setInterval(500);
    connect(alienAnimationTimer, SIGNAL(timeout()), this, SLOT(updateAliens()));

    gameUpdateTimer = new QTimer(this);
    gameUpdateTimer->setInterval(17);
    connect(gameUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));

    bulletUpdateTimer = new QTimer(this);
    bulletUpdateTimer->setInterval(15);
    connect(bulletUpdateTimer, SIGNAL(timeout()), this, SLOT(updateBullets()));

    alienBulletTimer = new QTimer(this);
    alienBulletTimer->setInterval(500);
    connect(alienBulletTimer, SIGNAL(timeout()), this, SLOT(alienFireSelect()));

    ufoSpawnTimer = new QTimer(this);
    ufoSpawnTimer->setInterval(100);
    connect(ufoSpawnTimer, SIGNAL(timeout()), this, SLOT(spawnUFO()));

    alienAnimationTimer->start();
    gameUpdateTimer->start();
    bulletUpdateTimer->start();
    alienBulletTimer->start();
    ufoSpawnTimer->start();
}

GameManager::~GameManager()
{
    if(player != NULL)
        delete player;

    for(int i = 0; i < 5; i++)
    {
        if(bullets[i] != NULL)
        {
            delete bullets[i];
            bullets[i] = NULL;
        }
    }

    foreach(Alien *a, alienVec)
    {
        if(a != NULL)
        {
            delete a;
        }
    }

    alienVec.clear();

    for(int i = 0; i < 15; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            grid[i][j] = 0;
        }
    }
}

void GameManager::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);

    paint.setBrush(QBrush(Qt::black));
    paint.drawRect(this->rect());

    if(!gameOver && !levelEnd)
    {
        paint.setPen(Qt::white);

        //=================
        // Grid Rendering
        //=================

        int posX = 15, posY = 70;
        for(int i = 0; i < ROWS; i++)
        {
            for(int j = 0; j < COLUMNS; j++)
            {
                if(!levelEnd)
                {
                    if(i >= invadersTopRow && i < invadersTopRow + 5)
                    {
                        if(j >= invadersLeftColumn && j < invadersRightColumn)
                            paint.drawText(posX + 5, posY + 15, QString("%1").arg(grid[i][j]));
                    }
                }

                posX += X_OFFSET;
            }

            posX = 15;
            posY += Y_OFFSET;
        }
        paint.setPen(QPen(Qt::green));
        paint.drawText(15, 35, QString("Left column: %1    Right column: %2    Top row: %3    Kill count: %4").arg(invadersLeftColumn).arg(invadersRightColumn).arg(invadersTopRow).arg(killCount));
        paint.drawLine((invadersLeftColumn * 30) + 15, (invadersTopRow * 40) + 70, (invadersLeftColumn * 30) + 15, ((invadersTopRow + 5) * 40) + 70);
        paint.drawLine((invadersRightColumn * 30) + 15, (invadersTopRow * 40) + 70, (invadersRightColumn * 30) + 15, ((invadersTopRow + 5) * 40) + 70);
        paint.drawLine((invadersLeftColumn * 30) + 15, (invadersTopRow * 40) + 70, (invadersRightColumn * 30) + 15, (invadersTopRow * 40) + 70);
        paint.setPen(QPen(Qt::white));

        //======================
        // Projectile Rendering
        //======================

        if(!levelEnd)
        {
            for(int i = 0; i < 5; i++)
            {
                if(bullets[i] != NULL)
                    bullets[i]->drawBullet(&paint);
            }
        }

        //=================
        // Enemy Rendering
        //=================

        if(!levelEnd)
        {
            paint.setBrush(QBrush(Qt::white));

            foreach(Alien *a, alienVec)
            {
                if(a != NULL)
                    a->drawAlien(&paint, redrawAliens, shiftAliens);
            }

            if(redrawAliens && shiftAliens)
            {
                shiftAliens = false;
            }

            if(UFO != NULL)
            {
                if(!UFO->isMarked())
                    UFO->drawUFO(&paint, updateUFO);

                if(UFO->GetPosX() < 30 && !UFO->StartedOnLeft())
                    UFO->MarkForDelete();
                else if(UFO->GetPosX() > 900 && UFO->StartedOnLeft())
                    UFO->MarkForDelete();

                if(updateUFO)
                    updateUFO = false;
            }
        }

        redrawAliens = false;

        //==================
        // Bunker Rendering
        //==================

        if(!levelEnd)
        {
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
        }

        //==================
        // Player Rendering
        //==================
        if(player != NULL)
        {
            player->UpdatePosition();
            player->drawPlayer(&paint);
        }

        //================
        // UI Rendering
        //================

        paint.drawLine(QLine(0, 720, 900, 720));
        QFont font("TypeWriter", 10, 1);
        paint.setFont(font);
        paint.drawText(20, 735, QString("%1").arg(player->GetLivesRemaining()));
        paint.drawText(20, 20, QString::number(playerScore));
        paint.drawText(380, 20, "<<ADD HIGH SCORE>>");
        /// Add high score tracker
    }
    else if(gameOver && !levelEnd)
    {
        paint.setPen(QPen(Qt::green));
        QFont font("Courier", 36, 1);
        paint.setFont(font);
        paint.drawText((this->width() / 2) - 130, this->height() / 2, QString("GAME OVER"));
        QFont font2("Courier", 25, 1);
        paint.setFont(font2);
        paint.drawText((this->width() / 2) - 310, this->height() / 2 + 50, QString("New Game? Press [Space] or [Enter]"));
    }
    else
    {
        paint.setPen(QPen(Qt::green));
        QFont font("Courier", 36, 1);
        paint.setFont(font);
        paint.drawText((this->width() / 2) - 150, this->height() / 2, QString("LEVEL CLEAR!"));
        QFont font2("Courier", 25, 1);
        paint.setFont(font2);
        paint.drawText((this->width() / 2) - 350, this->height() / 2 + 50, QString("Press [Space] or [Enter] to Continue"));

        if(player != NULL)
        {
            player->drawPlayer(&paint);
        }
    }
}

void GameManager::keyPressEvent(QKeyEvent *event)
{
    if((event->key() == Qt::Key_Space || event->key() == Qt::Key_Return) && (gameOver || levelEnd))
    {
        if(gameOver)
            SetupGame(true);
        else if(levelEnd)
            SetupGame(false);
    }
    else if(event->key() == Qt::Key_Escape)
    {
        this->close();
    }
    else
    {
        player->InputHandler(event, true);
    }

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

void GameManager::GameOver()
{
    gameOver = true;

    if(player != NULL)
        delete player;

    for(int i = 0; i < 5; i++)
    {
        if(bullets[i] != NULL)
        {
            delete bullets[i];
            bullets[i] = NULL;
        }
    }

    foreach(Alien *a, alienVec)
    {
        if(a != NULL)
        {
            delete a;
        }
    }

    alienVec.clear();

    for(int i = 0; i < 15; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            grid[i][j] = 0;
        }
    }

    for(int k = 0; k < 4; k++)
    {
        for(int i = 0; i < 30; i++)
        {
            for(int j = 0; j < 44; j++)
            {
                bunker[k][i][j] = bunkerRef[k][i][j];
            }
        }
    }

    if(UFO != NULL)
        delete UFO;

}

void GameManager::Victory()
{
    levelEnd = true;

    for(int i = 0; i < 5; i++)
    {
        if(bullets[i] != NULL)
        {
            delete bullets[i];
            bullets[i] = NULL;
        }
    }

    foreach(Alien *a, alienVec)
    {
        if(a != NULL)
        {
            delete a;
        }
    }

    alienVec.clear();

    for(int i = 0; i < 15; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            grid[i][j] = 0;
        }
    }

    for(int k = 0; k < 4; k++)
    {
        for(int i = 0; i < 30; i++)
        {
            for(int j = 0; j < 44; j++)
            {
                bunker[k][i][j] = bunkerRef[k][i][j];
            }
        }
    }

    if(UFO != NULL)
        delete UFO;
}

void GameManager::SetupGame(bool newGame)
{
    redrawAliens = true;
    redrawBunkers = true;
    shiftAliens = false;
    left = false;
    pauseGame = false;
    updateUFO = false;
    invadersTopRow = 0;
    invadersLeftColumn = 0;
    invadersRightColumn = 11;
    killCount = 0;
    player = new Player(422, 680);

    if(newGame)
        playerScore = 0;

    UFO = NULL;

    // Initialize the Aliens
    int alienX = 18, alienY = 80;
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 11; j++)
        {
            alienVec.push_back(new Alien(invaders[i][j], alienX, alienY));
            grid[i][j] = 1;
            alienX += X_OFFSET;
        }

        alienX = 18;
        alienY += Y_OFFSET;
    }

    levelEnd = false;
    gameOver = false;
}

void GameManager::updateBullets()
{
    if(!gameOver && !levelEnd)
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
                //=================
                // Alien Collision
                //=================
                if(h == 0)
                {
                    int grid_j = (bullets[h]->GetPosX() - 15) / X_OFFSET;
                    int grid_i = (bullets[h]->GetPosY() - 70) / Y_OFFSET;

                    // Find invader coord to determine type:
                    int invader_i = grid_i - abs(invadersTopRow);
                    int invader_j = grid_j - abs(invadersLeftColumn);

                    if((invader_i < 5) && (invader_j < 11) && (invader_i >= 0) && (invader_j >= 0))
                    {
                        if(grid[grid_i][grid_j] == 1)
                        {
                            int bulletRelPosX = bullets[h]->GetPosX() % 12;
                            int bulletRelPosY = bullets[h]->GetPosY() % 12;
                            int index = (invader_i * 11) + invader_j;
                            if(index >= 0 && alienVec.at(index) != NULL)
                            {
                                if(alienVec.at(index)->CheckCollision(bulletRelPosX, bulletRelPosY, invaders[invader_i][invader_j]))
                                {
                                    grid[grid_i][grid_j] = 0;
                                    alienVec.at(index)->kill();
                                    killCount++;
                                    deleteBullet = true;
                                    playerScore += (invaders[invader_i][invader_j] + 2) * 10;
                                }
                            }
                        }
                    }

                    if(UFO != NULL)
                    {
                        if((bullets[h]->GetPosY() <= 70) && (bullets[h]->GetPosY() >= 30))
                        {
                            if(UFO->CheckCollision(bullets[h]->GetPosX(), bullets[h]->GetPosY()))
                            {
                                UFO->MarkForDelete();
                                playerScore += 200;
                            }
                        }
                    }
                }

                //==================
                // Player Collision
                //==================
                if(h != 0)
                {
                    if(player->CheckCollision(bullets[h]->GetPosX(), bullets[h]->GetPosY()))
                    {
                        if(player->RemoveLife())
                        {
                            GameOver();
                            return;
                        }
                        else
                        {
                            player->SetPosition(422, 680);
                        }

                        deleteBullet = true;
                    }
                }

                // Bullet has reached edge of play area
                if(bullets[h]->GetPosY() <= 30 || deleteBullet || bullets[h]->GetPosY() >= 720)
                {
                    deleteBullet = false;
                    bullets[h] = NULL;

                    if(h == 0)
                    {
                        player->ResetLaser();
                    }
                    else
                    {
                        alienVec.at(alienFireTracker[h]);
                        alienFireTracker[h] = -1;
                    }
                }

                if(killCount == 55)
                {
                    Victory();
                    return;
                }
            }
        }
    }
}

void GameManager::updateAliens()
{
    if(!gameOver && !levelEnd)
    {
        bool skipCount = false;
        redrawAliens = true;

        if(redrawAliens && shiftAliens)
        {
            shiftAliens = false;
        }

        if(redrawAliens)
        {
            if(left)
            {
                for(int j = 0; j < COLUMNS; j++)
                {
                    for(int i = 0; i < ROWS; i++)
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

                if(!shiftAliens && !skipCount)
                {
                    invadersLeftColumn--;
                    invadersRightColumn--;
                    skipCount = true;
                }

            }
            else
            {
                for(int j = COLUMNS - 1; j >= 0; j--)
                {
                    for(int i = 0; i < ROWS; i++)
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

                if(!shiftAliens && !skipCount)
                {
                    invadersLeftColumn++;
                    invadersRightColumn++;
                    skipCount = true;
                }
            }
        }

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
                            GameOver();
                            return;
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
    }

}

void GameManager::alienFireSelect()
{
    if(!gameOver && !levelEnd)
    {
        int invader_i = rand() % 5;
        int invader_j = rand() % 11;
        if(alienVec.at((invader_i * 11) + invader_j) != NULL)
        {
            alienVec.at((invader_i * 11) + invader_j)->Fire();
            alienFireTracker[bulletIndex] = ((invader_i * 11) + invader_j);
        }
    }
}

void GameManager::spawnUFO()
{
    if(!gameOver && !levelEnd)
    {
        if((UFO != NULL) && (UFO->isMarked()))
            UFO = NULL;

        static int spawnCount = 0;

        if(spawnCount == 100)
        {
            spawnCount = 0;
            int spawn = rand() % 100;

            if((spawn < 10) && (UFO == NULL))
            {
                UFO = new ufo(2, 32, true);
            }
            else if((spawn > 90) && (UFO == NULL))
            {
                UFO = new ufo(896, 32, false);
            }
        }
        else if(UFO != NULL)
        {
            updateUFO = true;
        }
        else
        {
           spawnCount++;
        }
    }
}
