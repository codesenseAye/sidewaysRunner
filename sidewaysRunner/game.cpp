#ifndef GAME_H
#define GAME_H
#include "game.h"
#endif

#ifndef RENDERER_H
#define RENDERER_H
#include "renderer.h"
#endif

#include <conio.h>

#include "timercpp.h"
#include <windows.h>

#define KEY_UP 72 // define a macro 

template <typename lambda>
void game::start(lambda gameEnd) {
    int keyPressed = 0;

    Timer renderClock;
    Timer inputClock;

    renderer* r = new renderer; // make our new renderer
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
    SetConsoleTitle(TEXT("Sideways Runner"));
    //^ make sure we display all of the pixels that are going to be generated

    startGame();
    while (true); // Keep main thread active
}
