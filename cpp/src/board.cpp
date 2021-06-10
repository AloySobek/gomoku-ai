#include "board.hpp"
#include <chrono>

Board::Board()
{
    move_map[BOARD_SIZE / 2 * BOARD_SIZE + BOARD_SIZE / 2] = 1;
}

bool Board::place_stone_on_board(int8_t x, int8_t y, bool is_black, uint8_t *captures)
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
    {
        if (is_black)
        {
            black_board[y] |= 0x40000 >> x;
            if (captures)
            {
                if (y-3 >= 0 && (black_board[y-3] & white_board[y-2] & white_board[y-1] & (0x40000 >> x)))
                    remove_stone_from_board(x,y-1), remove_stone_from_board(x,y-2), *captures |= 0x1;
                if (y-3 >= 0 && x+3 < BOARD_SIZE && ((black_board[y-3] << 3) & (white_board[y-2] << 2) & (white_board[y-1] << 1) & (0x40000 >> x)))
                    remove_stone_from_board(x+1,y-1), remove_stone_from_board(x+2,y-2), *captures |= 0x2;
                if (x+3 < BOARD_SIZE && ((black_board[y] << 3) & (white_board[y] << 2) & (white_board[y] << 1) & (0x40000 >> x)))
                    remove_stone_from_board(x+1,y), remove_stone_from_board(x+2,y), *captures |= 0x4;
                if (y+3 < BOARD_SIZE && x+3 < BOARD_SIZE && ((black_board[y+3] << 3) & (white_board[y+2] << 2) & (white_board[y+1] << 1) & (0x40000 >> x)))
                    remove_stone_from_board(x+1,y+1), remove_stone_from_board(x+2,y+2), *captures |= 0x8;
                if (y+3 < BOARD_SIZE && (black_board[y+3] & white_board[y+2] & white_board[y+1] & (0x40000 >> x)))
                    remove_stone_from_board(x,y+1), remove_stone_from_board(x,y+2), *captures |= 0x10;
                if (y+3 < BOARD_SIZE && x-3 >= 0 && ((black_board[y+3] >> 3) & (white_board[y+2] >> 2) & (white_board[y+1] >> 1) & (0x40000 >> x)))
                    remove_stone_from_board(x-1,y+1), remove_stone_from_board(x-2,y+2), *captures |= 0x20;
                if (x-3 >= 0 && ((black_board[y] >> 3) & (white_board[y] >> 2) & (white_board[y] >> 1) & (0x40000 >> x)))
                    remove_stone_from_board(x-1,y), remove_stone_from_board(x-2,y), *captures |= 0x40;
                if (y-3 >= 0 && x-3 >= 0 && ((black_board[y-3] >> 3) & (white_board[y-2] >> 2) & (white_board[y-1] >> 1) & (0x40000 >> x)))
                    remove_stone_from_board(x-1,y-1), remove_stone_from_board(x-2,y-2), *captures |= 0x80;
            }
        }
        else
        {
            white_board[y] |= 0x40000 >> x;
            if (captures)
            {
                if (y-3 >= 0 && (white_board[y-3] & black_board[y-2] & black_board[y-1] & (0x40000 >> x)))
                    remove_stone_from_board(x,y-1), remove_stone_from_board(x,y-2), *captures |= 0x1;
                if (y-3 >= 0 && x+3 < BOARD_SIZE && ((white_board[y-3] << 3) & (black_board[y-2] << 2) & (black_board[y-1] << 1) & (0x40000 >> x)))
                    remove_stone_from_board(x+1,y-1), remove_stone_from_board(x+2,y-2), *captures |= 0x2;
                if (x+3 < BOARD_SIZE && ((white_board[y] << 3) & (black_board[y] << 2) & (black_board[y] << 1) & (0x40000 >> x)))
                    remove_stone_from_board(x+1,y), remove_stone_from_board(x+2,y), *captures |= 0x4;
                if (y+3 < BOARD_SIZE && x+3 < BOARD_SIZE && ((white_board[y+3] << 3) & (black_board[y+2] << 2) & (black_board[y+1] << 1) & (0x40000 >> x)))
                    remove_stone_from_board(x+1,y+1), remove_stone_from_board(x+2,y+2), *captures |= 0x8;
                if (y+3 < BOARD_SIZE && (white_board[y+3] & black_board[y+2] & black_board[y+1] & (0x40000 >> x)))
                    remove_stone_from_board(x,y+1), remove_stone_from_board(x,y+2), *captures |= 0x10;
                if (y+3 < BOARD_SIZE && x-3 >= 0 && ((white_board[y+3] >> 3) & (black_board[y+2] >> 2) & (black_board[y+1] >> 1) & (0x40000 >> x)))
                    remove_stone_from_board(x-1,y+1), remove_stone_from_board(x-2,y+2), *captures |= 0x20;
                if (x-3 >= 0 && ((white_board[y] >> 3) & (black_board[y] >> 2) & (black_board[y] >> 1) & (0x40000 >> x)))
                    remove_stone_from_board(x-1,y), remove_stone_from_board(x-2,y), *captures |= 0x40;
                if (y-3 >= 0 && x-3 >= 0 && ((white_board[y-3] >> 3) & (black_board[y-2] >> 2) & (black_board[y-1] >> 1) & (0x40000 >> x)))
                    remove_stone_from_board(x-1,y-1), remove_stone_from_board(x-2,y-2), *captures |= 0x80;
            }
        }

        for (int16_t y2 = y-1; y2 <= y+1; ++y2)
            for (int16_t x2 = x-1; x2 <= x+1; ++x2)
                if (x2 >= 0 && x2 < BOARD_SIZE && y2 >= 0 && y2 < BOARD_SIZE)
                    ++move_map[y2 * BOARD_SIZE + x2];

        return (true);
    }
    return (false);
}


bool Board::remove_stone_from_board(int8_t x, int8_t y, bool is_black, uint8_t *captures)
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
    {
        black_board[y] &= ~(0x40000 >> x);
        white_board[y] &= ~(0x40000 >> x);

        for (int16_t y2 = y-1; y2 <= y+1; ++y2)
            for (int16_t x2 = x-1; x2 <= x+1; ++x2)
                if (x2 >= 0 && x2 < BOARD_SIZE && y2 >= 0 && y2 < BOARD_SIZE)
                    --move_map[y2 * BOARD_SIZE + x2];

        if (captures)
        {
            if (*captures & 0x1)
                place_stone_on_board(x,y-1,!is_black), place_stone_on_board(x,y-2,!is_black);
            if (*captures & 0x2)
                place_stone_on_board(x+1,y-1,!is_black), place_stone_on_board(x+2,y-2,!is_black);
            if (*captures & 0x4)
                place_stone_on_board(x+1,y,!is_black), place_stone_on_board(x+2,y,!is_black);
            if (*captures & 0x8)
                place_stone_on_board(x+1,y+1,!is_black), place_stone_on_board(x+2,y+2,!is_black);
            if (*captures & 0x10)
                place_stone_on_board(x,y+1,!is_black), place_stone_on_board(x,y+2,!is_black);
            if (*captures & 0x20)
                place_stone_on_board(x-1,y+1,!is_black), place_stone_on_board(x-2,y+2,!is_black);
            if (*captures & 0x40)
                place_stone_on_board(x-1,y,!is_black), place_stone_on_board(x-2,y,!is_black);
            if (*captures & 0x80)
                place_stone_on_board(x-1,y-1,!is_black), place_stone_on_board(x-2,y-2,!is_black);
        }

        return (true);
    }
    return (false);
}

int32_t Board::minimax(int8_t depth, int32_t alpha, int32_t beta, bool maximizer, bool is_black)
{
    if (depth == 0 && ++nodes_count)
        return (std::rand() % 200000000);
    if (maximizer)
    {
        int32_t max_h = std::numeric_limits<int32_t>::min();
        for (uint16_t y{0}; y < BOARD_SIZE; ++y)
            for (uint16_t x{0}; x < BOARD_SIZE; ++x)
                if (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))
                    if (move_map[y * BOARD_SIZE + x])
                    {
                        uint8_t captures{0};
                        place_stone_on_board(x, y, is_black, &captures);
                        max_h = std::max(max_h, minimax(depth-1, alpha, beta, false, !is_black));
                        remove_stone_from_board(x, y, is_black, &captures);
                        alpha = std::max(alpha, max_h);
                        if (beta <= alpha && ++ pruned_count)
                            return max_h;
                    }
        return max_h;
    }
    else
    {
        int32_t min_h = std::numeric_limits<int32_t>::max();
        for (uint16_t y{0}; y < BOARD_SIZE; ++y)
            for (uint16_t x{0}; x < BOARD_SIZE; ++x)
                if (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))
                    if (move_map[y * BOARD_SIZE + x])
                    {
                        uint8_t captures{0};
                        place_stone_on_board(x, y, is_black, &captures);
                        min_h = std::min(min_h, minimax(depth-1, alpha, beta, true, !is_black));
                        remove_stone_from_board(x, y, is_black, &captures);
                        beta = std::min(beta, min_h);
                        if (beta <= alpha && ++pruned_count)
                            return min_h;
                    }
        return min_h;
    }
}

int32_t Board::ai_move(bool is_black)
{
    int32_t max_h{std::numeric_limits<int32_t>::min()};
    int32_t move{0};
    int32_t h{0};

    pruned_count = 0;
    nodes_count = 0;

    for (uint16_t y{0}; y < BOARD_SIZE; ++y)
        for (uint8_t x{0}; x < BOARD_SIZE; ++x)
            if (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (262144 >> x)))
                if (move_map[y * BOARD_SIZE + x])
                {
                    uint8_t captures{0};
                    place_stone_on_board(x, y, is_black, &captures);
                    h = minimax(3, std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max(), true, is_black);
                    remove_stone_from_board(x, y, is_black, &captures);
                    if (h >= max_h)
                    {
                        max_h = h;
                        move = 0;
                        move |= x;
                        move |= (y << 8);
                    }
                }
    return (move);
}

void Board::reset()
{
    for (int8_t i{0}; i < BOARD_SIZE; ++i)
    {
        black_board[i] = 0;
        white_board[i] = 0;
        for (int8_t j{0}; j < BOARD_SIZE; ++j)
            move_map[i * BOARD_SIZE + j] = 0;
    }
    move_map[BOARD_SIZE / 2 * BOARD_SIZE + BOARD_SIZE / 2] = 1;
}

void Board::print()
{
    for (uint16_t i{0}; i < BOARD_SIZE; ++i)
        std::cout << std::bitset<19>(black_board[i]) << "   " << std::bitset<19>(white_board[i]) << std::endl;
    for (uint16_t y{0}; y < BOARD_SIZE; ++y)
    {
        std::cout << "\t   ";
        for (uint16_t x{0}; x < BOARD_SIZE; ++x)
            std::cout << move_map[y * BOARD_SIZE + x];
        std::cout << "\n";
    }
}
