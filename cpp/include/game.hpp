#ifndef GAME_HPP
# define GAME_HPP

# include <chrono>

# include "board.hpp"

class Move {
public:
    explicit Move(bool valid, int8_t x=0, int8_t y=0, int8_t v=0, double t=0) : valid(valid), x(x), y(y), v(v), tookSecond(t) {}
    bool valid;
    int8_t x;
    int8_t y;
    int8_t v;
    double tookSecond;
};

class Game
{
public:
    int8_t getToken(int8_t x, int8_t y);
    bool setToken(int8_t x, int8_t y, int8_t v);
    Move predictMove(int8_t v);
    void reset();

private:
    Board board{Board()};
};

#endif
