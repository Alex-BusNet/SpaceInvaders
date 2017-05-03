#include "gamemanager.h"
#include <QDebug>
#include <QPainter>
#include <random>
#include <ctime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#define ROWS 15
#define COLUMNS 30
#define X_OFFSET 30
#define Y_OFFSET 40
#define SHIFT_AMOUNT 30
#define GRID_END ((X_OFFSET * 11) + (SHIFT_AMOUNT * (COLUMNS - 11)) + 15)

GameManager::GameManager(QWidget *parent, int currentHighScore) : QWidget(parent)
{
    this->setFixedSize(930, 750);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    srand(time(0));

    mp_player = new QMediaPlayer();
    mp_alienHit = new QMediaPlayer();
    mp_ufo = new QMediaPlayer();

    this->scoreToBeat = currentHighScore;

    alienAnimationTimer = new QTimer();
    alienAnimationTimer->setInterval(500);
    connect(alienAnimationTimer, SIGNAL(timeout()), this, SLOT(updateAliens()));

    gameUpdateTimer = new QTimer();
    gameUpdateTimer->setInterval(17);
    connect(gameUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));

    bulletUpdateTimer = new QTimer();
    bulletUpdateTimer->setInterval(15);
    connect(bulletUpdateTimer, SIGNAL(timeout()), this, SLOT(updateBullets()));

    alienBulletTimer = new QTimer();
    alienBulletTimer->setInterval(500);
    connect(alienBulletTimer, SIGNAL(timeout()), this, SLOT(alienFireSelect()));

    ufoSpawnTimer = new QTimer();
    ufoSpawnTimer->setInterval(100);
    connect(ufoSpawnTimer, SIGNAL(timeout()), this, SLOT(spawnUFO()));

    SetupGame(true);

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

//        int posX = 15, posY = 70;
//        for(int i = 0; i < ROWS; i++)
//        {
//            for(int j = 0; j < COLUMNS; j++)
//            {
//                if(!levelEnd)
//                {
//                    if(i >= invadersTopRow && i < invadersTopRow + 5)
//                    {
//                        if(j >= invadersLeftColumn && j < invadersRightColumn)
//                        {
//                            paint.drawText(posX + 5, posY + 15, QString("%1").arg(grid[i][j]));
//                            paint.drawText(posX + 20, posY + 15, (alienVec.at(((i - invadersTopRow) * 11) + (j - invadersLeftColumn))->isAlive() ? QString("A") : QString("D")));
//                        }
//                    }
//                }

//                posX += X_OFFSET;
//            }

//            posX = 15;
//            posY += Y_OFFSET;
//        }
//        paint.setPen(QPen(Qt::green));
//        paint.drawText(15, 35, QString("Left column: %1    Right column: %2    Top row: %3    Kill count: %4    Shots fired: %5").arg(invadersLeftColumn).arg(invadersRightColumn).arg(invadersTopRow).arg(killCount).arg(player->GetShotsFired()));
//        paint.drawLine((invadersLeftColumn * 30) + 15, (invadersTopRow * 40) + 70, (invadersLeftColumn * 30) + 15, ((invadersTopRow + 5) * 40) + 70);
//        paint.drawLine((invadersRightColumn * 30) + 15, (invadersTopRow * 40) + 70, (invadersRightColumn * 30) + 15, ((invadersTopRow + 5) * 40) + 70);
//        paint.drawLine((invadersLeftColumn * 30) + 15, (invadersTopRow * 40) + 70, (invadersRightColumn * 30) + 15, (invadersTopRow * 40) + 70);
//        paint.setPen(QPen(Qt::white));

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

            if(ufo != NULL)
            {
                if(!ufo->isMarked())
                    ufo->drawUFO(&paint, updateUFO);

                if(ufo->GetPosX() < 30 && !ufo->StartedOnLeft())
                    ufo->MarkForDelete();
                else if(ufo->GetPosX() > 900 && ufo->StartedOnLeft())
                    ufo->MarkForDelete();

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
        QFont font("Courier", 10, 1);
        paint.setFont(font);
        paint.drawText(20, 735, QString("%1").arg(player->GetLivesRemaining()));
        paint.drawText(20, 20, QString("SCORE 1<%1>").arg(playerScore, 6, 10, QChar('0')));
        paint.drawText(380, 20, QString("HIGHSCORE<%1>").arg(this->scoreToBeat, 6, 10, QChar('0')));
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

    if(killCount == 55 && ufo == NULL)
    {
        Victory();
        return;
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
        UpdateHighscores();
        this->close();
    }
//    else if(event->key() == Qt::Key_Right)
//    {
//        updateAliens();
//    }
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
        mp_player->setMedia(QUrl::fromLocalFile("../SpaceInvaders/Assets/Sound/player_shoot.wav"));
        mp_player->play();
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
    UpdateHighscores();

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

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 11; j++)
        {
            invaders[i][j] = invadersRef[i][j];
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

    if(ufo != NULL)
        delete ufo;

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

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 11; j++)
        {
            invaders[i][j] = invadersRef[i][j];
        }
    }

    if(ufo != NULL)
        delete ufo;
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
    player->ResetShotsFired(true);

    if(newGame)
    {
        playerScore = 0;
        levelCount = 1;
    }
    else
    {
        levelCount++;
    }

    ufo = NULL;

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

void GameManager::UpdateHighscores()
{
    QFile highscores("../SpaceInvaders/Assets/highscores.json");
    if(!highscores.open(QIODevice::ReadOnly))
    {
        qWarning("Could not open highscores file");
        return;
    }

    QByteArray byteArr = highscores.readAll();
    QJsonDocument hs = QJsonDocument::fromJson(byteArr);
    QJsonObject obj = hs.object();
    QJsonArray scores = obj["scores"].toArray();

    highscores.close();

    int scoreArr[10];

    for(int i = 0; i < 10; i++)
    {
        scoreArr[i] = scores.at(i).toInt();
    }

    scores.empty();

    int newScore = playerScore;
    for(int i = 9; i >= 0; i--)
    {
        if(newScore > scoreArr[i])
        {
            if(i > 0)
            {
                if(scoreArr[i - 1] != newScore)
                    scoreArr[i] = scoreArr[i-1];
            }
            else // i == 0
            {
                scoreArr[i] = newScore;
                break;
            }
        }
        else if(newScore < scoreArr[i])
        {
            if(i < 9)
            {
                if(scoreArr[i + 1] != newScore)
                {
                    scoreArr[i+1] = newScore;
                }
                break;
            }
            else // i == 9
                break;
        }
        else // newScore == scoreArr[i]
            break;
    }

    for(int i = 0; i < 10; i++)
    {
        scores.replace(i, scoreArr[i]);
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
                // Alien Collision
                //=================
                if(h == 0)
                {
                    int grid_j = (bullets[h]->GetPosX() - 15) / X_OFFSET;
                    int grid_i = (bullets[h]->GetPosY() - 70) / Y_OFFSET;

                    // Find invader coord to determine type:
                    int invader_i = grid_i - invadersTopRow;
                    int invader_j = grid_j - invadersLeftColumn;

                    if((invader_i < 5) && (invader_j < 11) && (invader_i >= 0) && (invader_j >= 0))
                    {
                        if(grid[grid_i][grid_j] == 1)
                        {
                            int index = (invader_i * 11) + invader_j;
                            if(index >= 0 && index < alienVec.size() && alienVec.at(index)->isAlive())
                            {
                                playerScore += (invaders[invader_i][invader_j] + 2) * 10;
                                grid[grid_i][grid_j] = 0;
                                invaders[invader_i][invader_j] = 3;
                                alienVec.at(index)->kill();
                                killCount++;
                                deleteBullet = true;

                                if(playerScore > scoreToBeat)
                                    scoreToBeat = playerScore;

                                mp_alienHit->setMedia(QUrl::fromLocalFile("../SpaceInvaders/Assets/Sound/invaderkilled.wav"));
                                mp_alienHit->play();
                            }
                        }
                    }

                    if(ufo != NULL)
                    {
                        if((bullets[h]->GetPosY() <= 70) && (bullets[h]->GetPosY() >= 30))
                        {
                            if(ufo->CheckCollision(bullets[h]->GetPosX(), bullets[h]->GetPosY()))
                            {
                                ufo->MarkForDelete();
                                playerScore += 200;
                            }
                        }
                    }
                }

                //=================
                // Bunker Collision
                //=================
                if(bullets[h]->GetPosY() >= 590 && bullets[h]->GetPosY() <= 650 && !deleteBullet)
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

                //==================
                // Player Collision
                //==================
                if(h != 0)
                {
                    if(player->CheckCollision(bullets[h]->GetPosX(), bullets[h]->GetPosY()))
                    {
                        mp_alienHit->setMedia(QUrl::fromLocalFile("../SpaceInvaders/Assets/Sound/explosion.wav"));
                        mp_alienHit->play();
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
        if((ufo != NULL) && (ufo->isMarked()))
            ufo = NULL;

        if(player->GetShotsFired() == 25)
        {
            player->ResetShotsFired(false);
            int spawn = rand() % 100;

            if((spawn < 50) && (ufo == NULL))
            {
                ufo = new UFO(2, 32, true);
            }
            else if((spawn > 49) && (ufo == NULL))
            {
                ufo = new UFO(896, 32, false);
            }
        }
        else if(ufo != NULL)
        {
            updateUFO = true;
            mp_ufo->setMedia(QUrl::fromLocalFile("../SpaceInvaders/Assets/Sound/ufo_highpitch.wav"));
            mp_ufo->setVolume(75);
            mp_ufo->play();
        }
    }
}


