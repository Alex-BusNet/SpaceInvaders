#include "ufo.h"
#include <QDebug>

UFO::UFO(int x, int y, bool leftSide)
{
    this->posX = x;
    this->posY = y;
    startOnLeft = leftSide;
    deleteLater = false;
    frame = 0;
}

void UFO::drawUFO(QPainter *paint, bool updateFrame)
{
    paint->setPen(QPen(Qt::red));
    paint->setBrush(QBrush(Qt::red));
    int rX = posX, rY = posY;
    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            if(ufo[frame][i][j] == 1)
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

bool UFO::CheckCollision(int bulletX, int bulletY)
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

int UFO::GetPosY()
{
    return posY;
}

int UFO::GetPosX()
{
    return posX;
}

bool UFO::StartedOnLeft()
{
    return startOnLeft;
}

void UFO::MarkForDelete()
{
    deleteLater = true;
}

bool UFO::isMarked()
{
    return deleteLater;
}
