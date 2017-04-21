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

void Alien::drawAlien(QPainter *paint, bool updateFrame)
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
