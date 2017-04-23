#include "bullet.h"
#include <QDebug>

Bullet::Bullet(bool player, int posX, int posY)
{
    this->player = player;
    this->posX = posX;
    this->posY = posY;
}

void Bullet::drawBullet(QPainter *painter)
{
    painter->drawLine(posX, posY, posX, posY+3);
}

void Bullet::UpdatePos()
{
    if(player)
    {
        posY -= 3;
    }else{
        posY += 3;
    }
}

