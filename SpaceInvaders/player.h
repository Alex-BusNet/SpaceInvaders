#ifndef PLAYER_H
#define PLAYER_H

#include <QPaintEvent>
#include <QPainter>
#include <QPolygon>

class Player
{
public:
    Player(int startX, int startY);
    void drawPlayer(QPainter *paint);
    int GetLivesRemaining();
    void InputHandler(QKeyEvent *e, bool isPressed);
    void UpdatePosition();
    void ResetLaser();
    bool CheckCollision(int bulletX, int bulletY);
    bool RemoveLife();
    void SetPosition(int posX, int posY);

private:
    void Fire();
    QPolygon laser;
    int posX, posY;
    int lives;
    int direction, lastDirection; // -1 = no movement, 0 = left, 1 = right
    bool laserReady, moveLeft, moveRight, isMovingLeft, isMovingRight;

    int playerRender[9][19] =
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
};

#endif // PLAYER_H
