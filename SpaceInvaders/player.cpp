#include "player.h"
#include "gamemanager.h"
#include <QDebug>

Player::Player(int startX, int startY)
{
    laserReady = true;
    posX = startX;
    posY = startY;
    lives = 3;
    direction = -1;
    isMovingLeft = false;
    isMovingRight = false;
    moveLeft = false;
    moveRight = false;
}

void Player::drawPlayer(QPainter *paint)
{
    int rX = posX, rY = posY;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 19; j++)
        {
            if(playerRender[i][j] == 1)
            {
                paint->drawRect(rX, rY, 2, 3);
            }

            rX += 3;
        }

        rX = posX;
        rY += 4;
    }
}

int Player::GetLivesRemaining()
{
    return lives;
}

void Player::InputHandler(QKeyEvent *e, bool isPressed)
{
    if(isPressed)
    {
        switch(e->key())
        {
        case Qt::Key_A:
            moveLeft = true;
            break;
        case Qt::Key_D:
            moveRight = true;
            break;
        case Qt::Key_Space:
            if(laserReady)
                Fire();
            break;
        default:
            break;
        }
    }
    else
    {
        switch(e->key())
        {
        case Qt::Key_A:
            moveLeft = false;
            break;
        case Qt::Key_D:
            moveRight = false;
            break;
        default:
            break;
        }
    }
}

void Player::UpdatePosition()
{
    if(moveLeft)
    {
        direction = 0;
        isMovingLeft = true;
        isMovingRight = false;
    }
    else if(moveRight)
    {
        direction = 1;
        isMovingLeft = false;
        isMovingRight = true;
    }
    else
    {
        // Stop moving
        isMovingLeft = false;
        isMovingRight = false;
        direction = -1;
    }

    if((0 == direction) && (posX >= 5) && isMovingLeft)
        posX -= 5;
    else if((1 == direction) && ((posX + 45) <= 925) && isMovingRight)
        posX += 5;
    else
        posX = posX;
}

void Player::ResetLaser()
{
    laserReady = true;
}

bool Player::CheckCollision(int bulletX, int bulletY)
{
    if(bulletX >= posX && bulletX <= posX + 45)
    {
        if(bulletY >= posY && bulletY <= posY + 30)
        {
            return true;
        }
    }

    return false;
}

bool Player::RemoveLife()
{
    lives--;
    if(lives <= 0)
    {
        return true;
    }

    return false;
}

void Player::SetPosition(int posX, int posY)
{
    this->posX = posX;
    this->posY = posY;
}

void Player::Fire()
{
    laserReady = false;
    GameManager::addBullet(true, posX + 28, posY + 6);
}

