// sidewaysRunner.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <conio.h>
#include <random>
#include <string> 

#include <vector>
#include <math.h>
#include "timercpp.h"
#include <windows.h>

#define _WIN32_WINNT 0x0500
#define KEY_UP 72 // define a macro 

using namespace std;

const int getRandomNum() { // returned random number cant be changed
    // static cast converts int return type to float type
    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 4000;
}

template <typename lambda>
void loopVectorArray(vector<vector<int>> array, lambda function) {
    for (int y = 0; y < array.size(); y++) {
        for (int x = 0; x < array.size(); x++) {
            vector<vector<int>> value = array[y][x];
            function(value);
        }
    }
}

enum locationTypes { // ints of types of locations
    nothingNum = 0,
    runnerNum = 1,
    obstacleNum = 2,
};


template<typename T>
class vector2 {
public:
    T x = 0;
    T y = 0;

    vector2(T pX = 0, T pY = 0) {
        x = pX;
        x = pY;
    }
};

class object {
protected:
    vector2<int> size;
};


class obstacle : object {
public:
    vector2<int> obstaclePos;
    vector2<int> obstacleSize;

    obstacle(int x = 0, int y = 0) {
        obstaclePos.x = x;
        obstaclePos.y = y;

        size.x = 1 + (rand() % 4);
        size.y = 1 + (rand() % 2);
    }
};

class runner : object {
public:
    int runnerOffset = 0; // for jumping

    void move();
    void jump(bool jumpState = false);
    
    vector2<int> runnerPos;
    vector2<int> runnerSize;

    runner() {}
};

class renderer {
    vector<obstacle> obstacles; // obstacles on the screen currently
    int obstacleNum = 0;
    int maxObstacles = 4;

    vector<int> screenSize = {100, 25};
public:
    void adjustScreen();
    void render(vector<vector<int>>& rendered);

    int screenPos = 0; // how far we've panned right in pixels
    runner ourRunner;
    
    renderer() {
        ourRunner.runnerSize.x = 2;
        ourRunner.runnerSize.y = 4;
    
        ourRunner.runnerPos.y = screenSize[1] - ourRunner.runnerSize.y;
        obstacles.resize(maxObstacles);
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


// move all positions so that they fit on the screen (delete all obstacles that are off screen)
void renderer::adjustScreen() {
    if (ourRunner.runnerPos.x + 1 >= screenSize[0]) {
        ourRunner.runnerPos.x = screenSize[0] - ourRunner.runnerPos.x;
        // set the screen size to be the leftovers of the screen size
    }

    int numObs = obstacles.size();

    for (unsigned int obN = 0; obN < numObs; obN++) { // unsigned = positive whole integer with double max value
        obstacle* ob = &obstacles[obN]; // dont copy the object with &

        if ((*ob).obstaclePos.x > screenPos) { // dereference the pointer to get the pos
            continue;
        }

        //obstacleNum -= 1;
        //delete ob; // not sure why this doesnt work yet
    }
}


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

    obstacles[obstacleNum - 1] = obstacle(x, screenSize[1] - height);
}

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
            
            int&y = obstacle.obstaclePos.x;
            int&x = obstacle.obstaclePos.y;

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

// render our list of objects;
void renderer::render(vector<vector<int>>& rendered) {
    system("CLS"); // clear the screen for the next render
    gameScore++;

    string score = "Score: " + std::to_string(gameScore);
    string failScore = "Failed: " + std::to_string(failedScore);

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
            string out;

            int scoreOffset = screenSize[0] - score.length();
            int failedOffset = screenSize[0] - failScore.length();
            
            if (y == 0 && x > scoreOffset) {
                if (x - 1 == scoreOffset) {
                    cout << score;
                }

                continue;
            }

            if (y == 1 && x > failedOffset) {
                if (x - 1 == failedOffset) {
                    cout << failScore;
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

            cout << out;
        }

        cout << "\n";
    }
}


bool renderer::isRunnerCollidingWithObstacle() const {
    // check if runner is colliding with obstacle

    return false;
}


void runner::move()
{
    // have the runner move forward
    runnerPos.x += 1;
}

void runner::jump(bool jumpState)
{
    // have the runner jump up
    if (jumpState) {
        runnerOffset = 5;
    }
    else {
        runnerOffset = 0;
    }
}

class game {
public:
    template <typename lambda>
    void start(lambda gameEnd);

};


template <typename lambda>
void game::start(lambda gameEnd) {
    int keyPressed = 0;
    
    Timer renderClock;
    Timer inputClock;

    renderer *r = new renderer; // make our new renderer
    renderer& render = *r; // dereference our renderer pointer so we can use it

    float lastMadeObstacle = clock();
    float nextObstacleTime = static_cast<float>(getRandomNum()); // convert int to float (type)

    renderClock.setInterval([&/*, &r, &lastMadeObstacle, &nextObstacleTime*/] {
        // [] is capture list (no other variables can be used), [&] all variables in scope, [&x] 1 specific variable, [&x, &y] 2 specific variables, [=, &x] 1 specific and all others as copies, [&, x] all references and 1 copy
        render.ourRunner.move();
        render.screenPos += 1;

        if (lastMadeObstacle + nextObstacleTime < clock()) { 
            lastMadeObstacle = clock();
            nextObstacleTime = (float)getRandomNum();

            render.makeObstacle();
        }

        render.adjustScreen();
        render.renderScreen();

        if (render.isRunnerCollidingWithObstacle()) {
            delete r;

            renderClock.stop();
            inputClock.stop();

            gameEnd();
        }
    }, 250.0f);

    inputClock.setInterval([&] {
        keyPressed = _getch();
        render.ourRunner.jump(keyPressed == KEY_UP);
    }, 50.0f);
}


void startGame() {
    game gameInst;

    gameInst.start([&] {
        startGame();
    });
}


int main()
{
    HWND console = GetConsoleWindow();
    MoveWindow(console, 0, 0, 900, 500, false);
    //^ make sure we display all of the pixels that are going to be generated

    startGame();
    while (true); // Keep main thread active
}
