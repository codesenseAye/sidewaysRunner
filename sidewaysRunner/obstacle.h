#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H
#include "dependencies.h"
#endif

class obstacle : object {
public:
    vector2<int> obstaclePos;

    vector2<int> obstacleSize;
    bool generated = false;

    obstacle(int x = 0, int y = 0) {
        obstaclePos.x = x;
        obstaclePos.y = y;

        size.x = 1 + (rand() % 4);
        size.y = 1 + (rand() % 2);
    }
};