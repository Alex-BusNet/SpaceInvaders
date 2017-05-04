#include "bullet.h"
#include <QDebug>

Bullet::Bullet(bool player, int posX, int posY)
{
    this->player = player;
    this->posX = posX;
    this->posY = posY;
    this->frame = 0;
}

void Bullet::drawBullet(QPainter *paint)
{
    if(player)
    {
        int rX = posX, rY = posY;
        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                if(bulletRender[i][j] == 1)
                {
                    paint->drawRect(rX, rY, 1, 1);
                }

                rX += 1;
            }

            rX = posX;
            rY += 1;
        }
    }
    else
    {
        int rX = posX, rY = posY;
        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                if(alienBulletRender[frame][i][j] == 1)
                {
                    paint->drawRect(rX, rY, 1, 1);
                }

                rX += 2;
            }

            rX = posX;
            rY += 2;
        }
    }

}

int Bullet::GetPosX()
{
    return posX;
}

int Bullet::GetPosY()
{
    if(player) return posY;
    else return posY + 5;
}

void Bullet::UpdatePos()
{
    if(player)
    {
        posY -= 9;
    }
    else
    {
        posY += 3;

        if(frame == 0)
            frame = 1;
        else
            frame = 0;
    }
}

