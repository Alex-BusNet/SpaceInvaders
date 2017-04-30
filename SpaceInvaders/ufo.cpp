#include "ufo.h"
#include <QDebug>

ufo::ufo(int x, int y, bool leftSide)
{
    this->posX = x;
    this->posY = y;
    startOnLeft = leftSide;
    deleteLater = false;
    frame = 0;
}

void ufo::drawUFO(QPainter *paint, bool updateFrame)
{
    paint->setPen(QPen(Qt::red));
    paint->setBrush(QBrush(Qt::red));
    int rX = posX, rY = posY;
    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            if(UFO[frame][i][j] == 1)
            {
                paint->drawRect(rX, rY, 2, 2);
            }

            rX += 3;
        }
        rX = posX;
        rY += 3;
    }

    if(updateFrame)
    {
        if(frame == 1)
            frame = 0;
        else
            frame = 1;

        if(startOnLeft)
            posX += 16;
        else
            posX -= 16;
    }
}

bool ufo::CheckCollision(int bulletX, int bulletY)
{
    if((bulletX >= posX )&& (bulletX <= (posX + 48)))
    {
        if((bulletY >= posY) && (bulletY <= (posY + 53)))
        {
            return true;
        }
    }

    return false;
}

int ufo::GetPosY()
{
    return posY;
}

int ufo::GetPosX()
{
    return posX;
}

bool ufo::StartedOnLeft()
{
    return startOnLeft;
}

void ufo::MarkForDelete()
{
    deleteLater = true;
}

bool ufo::isMarked()
{
    return deleteLater;
}
