#ifndef BULLET_H
#define BULLET_H
#include <QPainter>

class Bullet
{
public:
    Bullet(bool player, int posX, int posY);
    void drawBullet(QPainter* painter);
    void UpdatePos();

private:
    bool player;
    int posX, posY;


};

#endif // BULLET_H
