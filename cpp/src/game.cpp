#include "gomoku.hpp"
#include <limits>
#include <locale>

void ai_move(board_t *board)
{
    int32_t max_h = std::numeric_limits<int32_t>::min();
    int32_t alpha = std::numeric_limits<int32_t>::min();
    int32_t beta = std::numeric_limits<int32_t>::max();
    int16_t move_x{0};
    int16_t move_y{0};
    int32_t h;

    for (uint16_t y{0}; y < BOARD_SIZE; ++y)
        for (uint16_t x{0}; x < BOARD_SIZE; ++x)
            if (get_token(board, x, y) == EMPTY)
                if (board->move_map[y * BOARD_SIZE + x])
                {
                    place_stone_on_board(board, x, y, 1);
                    h = minimax(3, &alpha, &beta, board, 1);        
                    remove_stone_from_board(board, x, y, 1);
                    if (h >= max_h)
                    {
                        max_h = h;
                        move_x = x;
                        move_y = y;
                    }
                }
    place_stone_on_board(board, move_x, move_y, 1);
}

int8_t player_move(board_t *board, int16_t x, int16_t y)
{
    return (place_stone_on_board(board, x, y, 0));
}
