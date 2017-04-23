#ifndef BULLET_H
#define BULLET_H
#include <QPainter>

class Bullet
{
public:
    Bullet();
    Bullet(bool player, int posX, int posY);
    void drawBullet(QPainter* painter);
    int GetPosX();
    int GetPosY();
public slots:
    void UpdatePos();

private:
    bool player;
    int posX, posY, frame;
};

#endif // BULLET_H
