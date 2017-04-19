#include "player.h"
#include <QDebug>

Player::Player(int startX, int startY)
{
    posX = startX;
    posY = startY;
    lives = 3;
    direction = -1;
    isMovingLeft = false;
    isMovingRight = false;
    moveLeft = false;
    moveRight = false;

    // Player Bounding rect size: 45px X 30px
    laser << QPoint(posX, posY + 20)        // 0
          << QPoint(posX + 5, posY + 15)    // 1
          << QPoint(posX + 15, posY + 15)   // 2
          << QPoint(posX + 18, posY + 3)    // 3
          << QPoint(posX + 20, posY + 3)    // 3.5
          << QPoint(posX + 20, posY)        // 4
          << QPoint(posX + 25, posY)        // 5
          << QPoint(posX + 25, posY + 3)    // 6
          << QPoint(posX + 27, posY + 3)    // 6.5
          << QPoint(posX + 30, posY + 15)   // 7
          << QPoint(posX + 40, posY + 15)   // 8
          << QPoint(posX + 45, posY + 20)   // 9
          << QPoint(posX + 45, posY + 30)   // 10
          << QPoint(posX, posY + 30);       // 11
}

Player::drawPlayer(QPainter *paint)
{
    paint->drawPolygon(laser);
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
            fireLaser = true;
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
        case Qt::Key_Space:
            fireLaser = false;
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

    if((0 == direction) && (posX >= 55) && isMovingLeft)
        posX -= 5;
    else if((1 == direction) && (posX <= 795) && isMovingRight)
        posX += 5;
    else
        posX = posX;

    laser.clear();
    laser << QPoint(posX, posY + 20)        // 0
          << QPoint(posX + 5, posY + 15)    // 1
          << QPoint(posX + 15, posY + 15)   // 2
          << QPoint(posX + 18, posY + 3)    // 3
          << QPoint(posX + 20, posY + 3)    // 3.5
          << QPoint(posX + 20, posY)        // 4
          << QPoint(posX + 25, posY)        // 5
          << QPoint(posX + 25, posY + 3)    // 6
          << QPoint(posX + 27, posY + 3)    // 6.5
          << QPoint(posX + 30, posY + 15)   // 7
          << QPoint(posX + 40, posY + 15)   // 8
          << QPoint(posX + 45, posY + 20)   // 9
          << QPoint(posX + 45, posY + 30)   // 10
          << QPoint(posX, posY + 30);       // 11
}

