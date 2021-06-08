#include "Game.h"

int8_t Game::getToken(int x, int y) {
    if (x >= GSIZE || x < 0 || y >= GSIZE || y < 0)
        return -1;
    return board[y][x];
}

bool Game::setToken(int x, int y, int8_t v) {
    if (x >= GSIZE || x < 0 || y >= GSIZE || y < 0)
        return false;
    board[y][x] = v;
    return true;
}


void Game::reset() {
    for (auto & y : board) {
        for (signed char & x : y) {
            x = 0;
        }
    }
}

Move Game::predictMove(int8_t v) {
    for (int8_t y = 0; y < GSIZE; ++y) {
        for (int8_t x = 0; x < GSIZE; ++x) {
            if (!board[y][x])
                return Move(true, x, y, v, 0);
        }
    }
    return Move(false);
}

