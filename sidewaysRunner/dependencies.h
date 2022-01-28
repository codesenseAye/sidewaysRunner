inline extern const int getRandomNum() { // returned random number cant be changed
    // static cast converts int return type to float type
    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 4000;
}

extern enum locationTypes { // ints of types of locations
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