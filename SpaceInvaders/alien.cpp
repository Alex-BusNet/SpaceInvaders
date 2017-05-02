#include "alien.h"
#include <QDebug>
#include "gamemanager.h"
#define SIZE 3
Alien::Alien(int type, int posX, int posY)
{
    this->type = type;
    this->posX = posX - 30;
    this->posY = posY;
    this->frame = 1;
    direction = 1; // 0 = Left, 1 = Right
    this->canFire = true;
    alive = true;
}

void Alien::drawAlien(QPainter *paint, bool updateFrame, bool shiftDown)
{
    if(alive)
    {
        int rX = posX, rY = posY;
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 12; j++)
            {
                if(alienRenders[type][frame][i][j] == 1)
                {
                    paint->drawRect(QRect(rX, rY, SIZE - 1, SIZE));
                }

                rX += SIZE - 1;
            }

            rX = posX;
            rY += SIZE;
        }
    }

    if(updateFrame)
    {
        if(frame == 1)
        {
            if(type == 3)
                alive = false;
            else
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
                posX -= 30;
            }
            else if(direction == 1)
            {
                // Moving right
                posX += 30;
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
    if(alive)
    {
        if(alienRenders[type][frame][bulletY][bulletX] == 1)
            return true;
    }

    return false;
}

void Alien::Fire()
{
    if(canFire && alive)
    {
        canFire = false;
        GameManager::addBullet(false, posX + 15, posY + 18);
    }
}

void Alien::ResetFire()
{
    canFire = true;
}

bool Alien::isAlive()
{
    return alive;
}

void Alien::kill()
{
    type =  3;
    frame = 1;
}

void Alien::ResetAlien(int type, int posX, int posY)
{
    this->type = type;
    this->posX = posX;
    this->posY = posY;
    alive = true;
    frame = 0;
}
