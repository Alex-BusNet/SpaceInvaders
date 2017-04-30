#ifndef BULLET_H
#define BULLET_H
#include <QPainter>

class Bullet
{
public:
    Bullet();
    Bullet(bool player, int posX, int posY);
    void drawBullet(QPainter* paint);
    int GetPosX();
    int GetPosY();

public slots:
    void UpdatePos();

private:
    bool player;
    int posX, posY, frame;

    int bulletRender[5][2] =
    {
        {1,1},
        {1,1},
        {1,1},
        {1,1},
        {1,1}
    };

    int alienBulletRender[2][5][4] =
    {
        //animation 1
        {
            {0,1,0,0},
            {1,1,1,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        },
        //animation 2
        {
            {0,0,1,0},
            {0,1,1,1},
            {0,0,1,0},
            {1,1,1,1},
            {0,0,1,0}
        }
    };
};

#endif // BULLET_H
