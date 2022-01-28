#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>


class Timer {
    std::atomic<bool> active{ true };

public:
    template<typename Function> // like generic types in lua except its above the whole function / class / struct and local to it?
    inline void setTimeout(Function function, int delay);

    template<typename Function>
    inline void setInterval(Function function, int interval);
    inline void stop();

};

template<typename Function>
inline void Timer::setTimeout(Function function, int delay) {
    active = true;
    std::thread t([=]() {
        if (!active.load()) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if (!active.load()) return;
        function();
        });
    t.detach();
}

template<typename Function>
inline void Timer::setInterval(Function function, int interval) {
    active = true;
    std::thread t([=]() {
        while (active.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if (!active.load()) return;
            function();
        }
        });
    t.detach();
}

inline void Timer::stop() {
    active = false;
}