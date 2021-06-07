#include "gomoku.hpp"

int8_t get_token(board_t *board, int16_t x, int16_t y)
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
    {
        if (board->black_board[y] & 262144 >> x)
            return BLACK_STONE;
        else if (board->white_board[y] & 262144 >> x)
            return WHITE_STONE;
        else
            return EMPTY;
    }
    return (OUT_OF_RANGE);
}

int8_t place_stone_on_board(board_t *board, int16_t x, int16_t y, int8_t is_black)
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
    {
        if (is_black)
            board->black_board[y] |= 262144 >> x;
        else
            board->white_board[y] |= 262144 >> x;

        for (int16_t y2 = y-1; y2 <= y+1; ++y2)
            for (int16_t x2 = x-1; x2 <= x+1; ++x2)
                if (x2 >= 0 && x2 < BOARD_SIZE && y2 >= 0 && y2 < BOARD_SIZE)
                    ++board->move_map[y2 * BOARD_SIZE + x2];
        return (1);
    }
    return (0);
}

int8_t remove_stone_from_board(board_t *board, int16_t x, int16_t y, int8_t is_black)
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
    {
        if (is_black)
            board->black_board[y] &= ~(262144 >> x);
        else
            board->white_board[y] &= ~(262144 >> x);

        for (int16_t y2 = y-1; y2 <= y+1; ++y2)
            for (int16_t x2 = x-1; x2 <= x+1; ++x2)
                if (x2 >= 0 && x2 < BOARD_SIZE && y2 >= 0 && y2 < BOARD_SIZE)
                    --board->move_map[y2 * BOARD_SIZE + x2];
        return (1);
    }
    return (0);
}

int32_t five_in_a_row(board_t *board)
{
    int32_t pattern = 0x1F;

    for (uint8_t i{0}; i < BOARD_SIZE; ++i)
    {
        pattern = 0x1F;
        for (uint8_t _{0};_ <= 14; ++_, pattern <<= 1)
            if ((board->black_board[i] & pattern) == pattern || (board->white_board[i] & pattern) == pattern)
                return FIVE_IN_A_ROW_SCORE;
        if (i+4 < BOARD_SIZE)
            if ((board->black_board[i] & board->black_board[i+1]
                    & board->black_board[i+2] & board->black_board[i+3]
                    & board->black_board[i+4]) || (board->black_board[i]
                    & (board->black_board[i+1] << 1) & (board->black_board[i+2] << 2)
                    & (board->black_board[i+3] << 3) & (board->black_board[i+4] << 4))
                    || (board->black_board[i] & (board->black_board[i+1] >> 1)
                    & (board->black_board[i+2] >> 2) & (board->black_board[i+3] >> 3)
                    & (board->black_board[i+4] >> 4)) || (board->white_board[i] & board->white_board[i+1]
                    & board->white_board[i+2] & board->white_board[i+3]
                    & board->white_board[i+4]) || (board->white_board[i]
                    & (board->white_board[i+1] << 1) & (board->white_board[i+2] << 2)
                    & (board->white_board[i+3] << 3) & (board->white_board[i+4] << 4))
                    || (board->white_board[i] & (board->white_board[i+1] >> 1)
                    & (board->white_board[i+2] >> 2) & (board->white_board[i+3] >> 3)
                    & (board->white_board[i+4] >> 4))
                    )
                return (FIVE_IN_A_ROW_SCORE);
    }
    return (0);
}

int32_t eval_board(board_t *board)
{
    int32_t score{0};

    srand(time(nullptr));
    score += rand() % 200000;

    if (board->black_board[0])
        board->black_board[0] = board->black_board[0];

    return (score);
}

void print_board(board_t *board)
{
    for (uint16_t i{0}; i < BOARD_SIZE; ++i)
        std::cout << std::bitset<19>(board->black_board[i]) << "   " << std::bitset<19>(board->white_board[i]) << std::endl;
    for (uint16_t y{0}; y < BOARD_SIZE; ++y)
    {
        std::cout << "\t   ";
        for (uint16_t x{0}; x < BOARD_SIZE; ++x)
            std::cout << board->move_map[y * BOARD_SIZE + x];
        std::cout << "\n";
    }
}
