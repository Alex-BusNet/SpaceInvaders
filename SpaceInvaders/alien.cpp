#include "alien.h"

Alien::Alien(int type, int posX, int posY)
{
    this->type = type;
    this->posX = posX;
    this->posY = posY;
    this->frame = 0;
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
                paint->drawRect(QRect(rX, rY, 3, 3));
            }

            rX += 3;
        }

        rX = posX;
        rY += 3;
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
    }
}
