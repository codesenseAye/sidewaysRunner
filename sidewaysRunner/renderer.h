#include <vector>

#ifndef OBSTACLES_H
#define OBSTACLES_H
#include "obstacle.h"
#endif

#include <string>
#include "runner.h"
using namespace std;

class renderer {
    vector<obstacle> obstacles; // obstacles on the screen currently
    int obstacleNum = 0;
    int maxObstacles = 4;

    bool rendering = true;

    vector<int> screenSize = { 100, 25 };
public:
    void adjustScreen();
    void render(vector<vector<int>>& rendered);
    void output(std::string toOutput);

    int screenPos = 0; // how far we've panned right in pixels
    runner ourRunner;

    renderer() {
        ourRunner.runnerSize.x = 2;
        ourRunner.runnerSize.y = 4;

        ourRunner.runnerPos.y = screenSize[1] - ourRunner.runnerSize.y;
    }

    void makeObstacle();
    void renderScreen();

    bool isRunnerCollidingWithObstacle() const;

    int failedScore = 0;
    int gameScore = 0;

    /*virtual*/ ~renderer() {
        // do destruction stuff
    }
};