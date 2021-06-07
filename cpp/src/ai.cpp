#include "gomoku.hpp"

int32_t minimax(int8_t depth, int32_t *alpha, int32_t *beta, board_t *board, int8_t is_black)
{
    if (depth == 0 or five_in_a_row(board))
        return eval_board(board);
    if (is_black)
    {
        int32_t max_h = std::numeric_limits<int32_t>::min();

        for (uint16_t y{0}; y < BOARD_SIZE; ++y)
            for (uint16_t x{0}; x < BOARD_SIZE; ++x)
                if (get_token(board, x, y) == EMPTY)
                    if (board->move_map[y * BOARD_SIZE + x])
                    {
                        place_stone_on_board(board, x, y, 0);
                        max_h = std::max(max_h, minimax(depth-1, alpha, beta, board, 0));        
                        remove_stone_from_board(board, x, y, 0);
                        *alpha = std::max(*alpha, max_h);
                        if (max_h >= *beta)
                            break;
                    }
        return max_h;
    }
    else
    {
        int32_t min_h = std::numeric_limits<int32_t>::max();

        for (uint16_t y{0}; y < BOARD_SIZE; ++y)
            for (uint16_t x{0}; x < BOARD_SIZE; ++x)
                if (get_token(board, x, y) == EMPTY)
                    if (board->move_map[y * BOARD_SIZE + x])
                    {
                        place_stone_on_board(board, x, y, 0);
                        min_h = std::min(min_h, minimax(depth-1, alpha, beta, board, 0));        
                        remove_stone_from_board(board, x, y, 0);
                        *beta = std::max(*beta, min_h);
                        if (min_h >= *alpha)
                            break;
                    }
        return min_h;
    }
}
