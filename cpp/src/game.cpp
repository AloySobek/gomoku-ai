#include "game.hpp"
#include <chrono>

int8_t Game::getToken(int8_t x, int8_t y)
{
    if (board.black_board[y] & 262144 >> x)
        return (BLACK_STONE);
    else if (board.white_board[y] & 262144 >> x)
        return (WHITE_STONE);
    else
        return (EMPTY_STONE);
}

bool Game::setToken(int8_t x, int8_t y, int8_t v)
{
    uint8_t stub;

    if (v == BLACK_STONE)
        return(board.place_stone_on_board(x,y,true,&stub));
    else if (v == WHITE_STONE)
        return(board.place_stone_on_board(x,y,false,&stub));
    else
        return(board.remove_stone_from_board(x,y,true));
}

Move Game::predictMove(int8_t v)
{
    int32_t move{0};
    auto start{std::chrono::high_resolution_clock::now()};
    move = v == BLACK_STONE ? board.ai_move(true) : board.ai_move(false);
    auto finish{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> elapsed = finish - start;
    return Move(true, (move & 0xFF), (move & 0xFF10) >> 8, v, elapsed.count());
}

void Game::reset()
{
    board.reset();
}
