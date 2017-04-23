#ifndef BULLET_H
#define BULLET_H
#include <QPainter>

class Bullet
{
public:
    Bullet();
    Bullet(bool player, int posX, int posY);
    void drawBullet(QPainter* painter);
    int posX, posY;

public slots:
    void UpdatePos();

private:
    bool player;


};

#endif // BULLET_H
