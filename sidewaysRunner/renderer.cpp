// sidewaysRunner.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <string> 

#include <vector>
#include <math.h>
#include "timercpp.h"
#include <windows.h>

#include "renderer.h"
#include <string>

#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H
#include "dependencies.h"
#endif

#ifndef OBSTACLES_H
#define OBSTACLES_H
#include "obstacle.h"
#endif

#define _WIN32_WINNT 0x0500
using namespace std;


// move all positions so that they fit on the screen (delete all obstacles that are off screen)
void renderer::adjustScreen() {
    if (ourRunner.runnerPos.x + 1 >= screenSize[0]) {
        ourRunner.runnerPos.x = screenSize[0] - ourRunner.runnerPos.x;
        // set the screen size to be the leftovers of the screen size
    }
}


//
void renderer::makeObstacle()
{
    // make an obstacle on the screen
    srand(time(0));

    int x = rand() % 10; // up to 10 pixels off screen
    x += screenPos + screenSize[0];
    // push off the screen + the random offset

    srand(time(0));
    int height = rand() % 3; // up to 3 pixels tall

    obstacleNum++;

    if (obstacleNum > maxObstacles) {
        return;
    }

    obstacle ob = obstacle(x, screenSize[1] - height);
    obstacles.resize(obstacleNum);

    ob.generated = true;
    obstacles[obstacleNum - 1] = ob;
}


//
void renderer::renderScreen()
{
    vector<vector<int>> rendered;
    rendered.resize(screenSize[1] * 2); // give room for obstacles that are on their way

    for (int y = 0; y < screenSize[1]; y++) {
        // y screen height
        vector<int> yList = {};
        yList.resize(screenSize[0] * 2);

        rendered[y] = yList;

        for (int x = 0; x < screenSize[0]; x++) {
            //cout << x << "\n";
            yList[x] = locationTypes::nothingNum; // nothing at this pos
        }
    }
    
    int& runnerOffset = ourRunner.runnerOffset;
    rendered[ourRunner.runnerPos.y - runnerOffset][ourRunner.runnerPos.x] = locationTypes::runnerNum; // our runner is here

    for (int y = 0; y < ourRunner.runnerSize.y; y++) {
        for (int x = 0; x < ourRunner.runnerSize.x; x++) {
            rendered[ourRunner.runnerPos.y - runnerOffset + y][ourRunner.runnerPos.x + x] = locationTypes::runnerNum; // our runner is here too
            // define the size our runner
        }
    }
    
    // render all of the obstacles and the runner on the screen
    int numObstacles = obstacles.size();

    if (numObstacles > 1) {
        // no obstacles on screen dont render it
        for (int i = 0; i < obstacles.size(); i++) {
            obstacle&obstacle = obstacles[i]; // use & to not make a copy of the object and just get the current reference
            
            if (!obstacle.generated) {
                continue;
            }

            int&x = obstacle.obstaclePos.x;
            int&y = obstacle.obstaclePos.y;

            int ySize = rendered.size();

            if (ySize <= y) {
                continue;
            }

            int xSize = rendered[y].size();

            if (xSize <= x) {
                continue;
            }

            rendered[y][x] = locationTypes::obstacleNum; // an obstacle is here

            for (int y = 0; y < obstacle.obstacleSize.y; y++) {
                for (int x = 0; x < obstacle.obstacleSize.x; x++) {
                    rendered[obstacle.obstaclePos.y + y][obstacle.obstaclePos.x + x] = locationTypes::obstacleNum; // an obstacle is here too
                    // define the size our runner
                }
            }
        }
    }

    render(rendered);
}


void renderer::output(std::string toOutput) {
    if (rendering) {
        cout << toOutput;
    }
}


// render our list of objects;
void renderer::render(vector<vector<int>>& rendered) {
    if (rendering) {
        system("CLS"); // clear the screen for the next render
    }
    
    gameScore++;

    std::string score = "Score: " + std::to_string(gameScore);
    std::string failScore = "Failed: " + std::to_string(failedScore);

    for (auto y = 0; y < rendered.size(); y++) {
        vector<int> yList = rendered[y];
        int minScreenPos = max(screenPos - 9, 0); // offset the screen by some so the runner has some room

        if (y > screenSize[1]) {
            break;
        }

        for (int x = minScreenPos; x < rendered[y].size(); x++) {
            if (x > screenSize[0]) {
                break;
            }
            
            int& atPos = rendered[y][x];
            std::string out;

            int scoreOffset = screenSize[0] - score.length();
            int failedOffset = screenSize[0] - failScore.length();
            
            if (y == 0 && x > scoreOffset) {
                if (x - 1 == scoreOffset) {
                    output(score);
                }

                continue;
            }

            if (y == 1 && x > failedOffset) {
                if (x - 1 == failedOffset) {
                    output(failScore);
                }

                continue;
            }

            if (atPos == locationTypes::nothingNum) {
                out = "|";
            }
            else if (atPos == locationTypes::runnerNum) {
                out = "O";
            }
            else if (atPos == locationTypes::obstacleNum) {
                out = "X";
            }
            else {
                out = "?";
            };

            output(out);
        }

        output("\n");
    }
}


//
bool renderer::isRunnerCollidingWithObstacle() const {
    // check if runner is colliding with obstacle

    return false;
}