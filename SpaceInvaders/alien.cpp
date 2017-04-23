#include "alien.h"
#define SIZE 3
Alien::Alien(int type, int posX, int posY)
{
    this->type = type;
    this->posX = posX;
    this->posY = posY;
    this->frame = 0;
    direction = 1; // 0 = Left, 1 = Right
}

void Alien::drawAlien(QPainter *paint, bool updateFrame, bool shiftDown)
{
    int rX = posX, rY = posY;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 12; j++)
        {
            if(alienRenders[type][frame][i][j] == 1)
            {
                paint->drawRect(QRect(rX, rY, SIZE, SIZE));
            }

            rX += SIZE;
        }

        rX = posX;
        rY += SIZE;
    }

    if(updateFrame)
    {
        if(frame == 1)
        {
            frame = 0;
        }
        else
        {
            frame = 1;
        }

        if(shiftDown)
        {
            posY += 40;

            if(direction == 1)
            {
                direction = 0;
            }
            else
            {
                direction = 1;
            }
        }
        else
        {
            if(direction == 0)
            {
                // Moving left
                posX -= 20;
            }
            else if(direction == 1)
            {
                // Moving right
                posX += 20;
            }
        }
    }
}

void Alien::shiftDown()
{
    posY += 40;

    if(direction == 1)
    {
        direction = 0;
    }
    else
    {
        direction = 1;
    }
}

bool Alien::CheckCollision(int bulletX, int bulletY, int type)
{
    if(alienRenders[type][frame][bulletY][bulletX] == 1)
        return true;
    else
        return false;
}

void Alien::Fire()
{
    /// if(canFire)
    /// {
    ///     canFire = false;
    ///     GameManager::addBullet(false, posX + 20, y); <- Y is the bottom center of the alien.
    ///                                                     Still need to figure out how to calculate
    ///                                                       this.
    /// }
}

void Alien::ResetFire()
{
    canFire = true;
}
