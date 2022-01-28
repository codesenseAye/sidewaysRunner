#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H
#include "dependencies.h"
#endif

class runner : object {
public:
    int runnerOffset = 0; // for jumping

    inline void move();
    inline void jump(bool jumpState = false);

    vector2<int> runnerPos;
    vector2<int> runnerSize;

    runner() {}
};


inline void runner::move()
{
    // have the runner move forward
    runnerPos.x += 1;
}

inline void runner::jump(bool jumpState)
{
    // have the runner jump up
    if (jumpState) {
        runnerOffset = 5;
    }
    else {
        runnerOffset = 0;
    }
}
