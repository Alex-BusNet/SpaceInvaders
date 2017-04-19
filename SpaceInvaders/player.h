#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>
#include <QPolygon>



class Player
{
public:
    Player(int startX, int startY);
    drawPlayer(QPainter *paint);
    int GetLivesRemaining();
private:
    QPolygon laser;
    int posX, posY;
    int lives;
};

#endif // PLAYER_H
