//
// Created by misha on 07.06.2021.
//

#ifndef FT_GOMOKU_QT_GAME_H
#define FT_GOMOKU_QT_GAME_H

#include <cstdint>
#ifndef GSIZE
#define GSIZE 19
#endif

class Move {
public:
    explicit Move(bool valid, int8_t x=0, int8_t y=0, int8_t v=0, float t=0) : x(x), y(y), valid(valid), v(v), tookSecond(t) {}
    bool valid;
    int8_t x;
    int8_t y;
    int8_t v;
    float tookSecond;
};

class Game {
public:
    int8_t getToken(int x, int y);
    // Returns false if move is not valid
    bool setToken(int x, int y, int8_t v);

    Move predictMove(int8_t v);
    void reset();
private:
    int8_t board[GSIZE][GSIZE] = {0};
};


#endif //FT_GOMOKU_QT_GAME_H
