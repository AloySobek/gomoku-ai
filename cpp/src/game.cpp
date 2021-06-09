#include "game.hpp"
#include <chrono>

int8_t Game::getToken(int8_t x, int8_t y)
{
    return (board.get_stone(x, y));
}

bool Game::setToken(int8_t x, int8_t y, int8_t v)
{
    uint8_t stub;

    if (x == 10 && y == 5)
        board.print();

    bool result;

    if (v == BLACK_STONE)
        result = (board.place_stone_on_board(x,y,true,&stub));
    else if (v == WHITE_STONE)
        result = (board.place_stone_on_board(x,y,false,&stub));
    else
        result = (board.remove_stone_from_board(x,y,true));

    if (x == 10 && y == 5)
    {
        board.print();
        exit(0);
    }

    return result;
}

Move Game::predictMove(int8_t v)
{
    int32_t move;

    auto start = std::chrono::high_resolution_clock::now();

    if (v == BLACK_STONE)
        move = board.ai_move(true);
    else
        move = board.ai_move(false);

    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;

    return Move(true, (move & 0xFF), (move & 0xFF10) >> 8, v, elapsed.count());
}

void Game::reset()
{
    board.reset();
}
