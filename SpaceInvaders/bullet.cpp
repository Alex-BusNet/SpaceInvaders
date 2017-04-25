#include "bullet.h"
#include <QDebug>

Bullet::Bullet(bool player, int posX, int posY)
{
    this->player = player;
    this->posX = posX;
    this->posY = posY;
    this->frame = 0;
}

void Bullet::drawBullet(QPainter *painter)
{
    painter->drawLine(posX, posY, posX, posY + 5);
}

int Bullet::GetPosX()
{
    return posX + 2;
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
        posY -= 5;
    }
    else
    {
        posY += 5;
    }


}

