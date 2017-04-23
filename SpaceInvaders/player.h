#ifndef PLAYER_H
#define PLAYER_H

#include <QPaintEvent>
#include <QPainter>
#include <QPolygon>



class Player
{
public:
    Player(int startX, int startY);
    drawPlayer(QPainter *paint);
    int GetLivesRemaining();
    void InputHandler(QKeyEvent *e, bool isPressed);
    void UpdatePosition();
    void ResetLaser();

private:
    void Fire();
    QPolygon laser;
    int posX, posY;
    int lives;
    int direction, lastDirection; // -1 = no movement, 0 = left, 1 = right
    bool laserReady, moveLeft, moveRight, isMovingLeft, isMovingRight;

};

#endif // PLAYER_H
