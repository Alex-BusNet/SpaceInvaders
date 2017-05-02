#ifndef ALIENS_H
#define ALIENS_H

#include <QPainter>

class Alien
{
public:
    Alien(int type, int posX, int posY);
    void drawAlien(QPainter *paint, bool updateFrame, bool shiftDown);
    void shiftDown();
    bool CheckCollision(int bulletX, int bulletY, int type);
    void Fire();
    void ResetFire();
    bool isAlive();
    void kill();
    void ResetAlien(int type, int posX, int posY);

private:
    int type, posX, posY, frame, direction;
    bool canFire, alive;
    int alienRenders[4][2][8][12] = //[type][frame][i][j]
    {
        // type 0, animation 0
        {
            // animation 0
            {
                {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
                {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0},
                {0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0},
                {0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0}
            },
            // animation 1
            {
                {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
                {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
                {0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0},
                {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}
            }
        },
        // type 1
        {
            // animation 0
            {
                {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
                {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                {0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
                {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0},
                {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0}
            },
            // animation 1
            {
                {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
                {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
                {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}
            },
        },
        // type 2
        {
            // animation 0
            {
                {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                {0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0},
                {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                {0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0},
                {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0}
            },
            // animation 1
            {
                {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                {0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0},
                {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
                {0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0},
                {0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0}
            }
        },
        // type 3 (dead)
        {
            // animation 0
            {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
            },
            // animation 1
            {
                {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
                {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0},
                {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0},
                {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1}
            }
        }
    };
};

#endif // ALIENS_H
