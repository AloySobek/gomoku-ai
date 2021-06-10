#include "board.hpp"
#include <chrono>

int8_t Board::get_stone(int8_t x, int8_t y)
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
    {
        if (black_board[y] & 262144 >> x)
            return (BLACK_STONE);
        else if (white_board[y] & 262144 >> x)
            return (WHITE_STONE);
        else
            return (EMPTY_STONE);
    }
    return (std::numeric_limits<uint8_t>::max());
}

bool Board::place_stone_on_board(int8_t x, int8_t y, bool is_black, uint8_t *captures)
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
    {
        if (is_black)
        {
            black_board[y] |= 262144 >> x;

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
            white_board[y] |= 262144 >> x;

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
        black_board[y] &= ~(262144 >> x);
        white_board[y] &= ~(262144 >> x);

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

bool Board::five_in_a_row(bool is_black)
{
    int32_t *board = is_black ? black_board : white_board;

    for (uint8_t i{0}; i < BOARD_SIZE; ++i)
    {
        int32_t pattern = 0x1F;
        for (uint8_t _{0};_ <= 14; ++_, pattern <<= 1)
            if ((board[i] & pattern) == pattern)
                return (true);

        if (i+4 < BOARD_SIZE)
            if ((board[i] & board[i+1]
                    & board[i+2] & board[i+3]
                    & board[i+4]) || (board[i]
                    & (board[i+1] << 1) & (board[i+2] << 2)
                    & (board[i+3] << 3) & (board[i+4] << 4))
                    || (board[i] & (board[i+1] >> 1)
                    & (board[i+2] >> 2) & (board[i+3] >> 3)
                    & (board[i+4] >> 4)))
                return (true);
    }
    return (0);
}

int32_t Board::eval(bool is_black)
{
    int32_t score{0};

    /* five_in_a_row(is_black); */

    score += std::rand() % 200000000;

    return (score);
}

int32_t Board::ai_move(bool is_black)
{
    int32_t max_h{std::numeric_limits<int32_t>::min()};

    int32_t alpha{std::numeric_limits<int32_t>::min()};
    int32_t beta{std::numeric_limits<int32_t>::max()};

    int32_t move{0};

    int32_t h{0};

    for (uint16_t y{0}; y < BOARD_SIZE; ++y)
        for (uint8_t x{0}; x < BOARD_SIZE; ++x)
            if (get_stone(x, y) == EMPTY_STONE)
                if (move_map[y * BOARD_SIZE + x])
                {
                    uint8_t captures{0};

                    place_stone_on_board(x, y, is_black, &captures);
                    h = minimax(3, &alpha, &beta, true, is_black);
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

int32_t Board::minimax(int8_t depth, int32_t *alpha, int32_t *beta, bool maximizer, bool is_black)
{
    if (depth == 0)// or five_in_a_row(is_black))
    {
        leafVisited++;
        return (eval(is_black) * (maximizer ? 1 : -1));
    }
    if (maximizer)
    {
        int32_t max_h = std::numeric_limits<int32_t>::min();

        for (uint16_t y{0}; y < BOARD_SIZE; ++y)
            for (uint16_t x{0}; x < BOARD_SIZE; ++x)
                if (get_stone(x, y) == EMPTY_STONE)
                    if (move_map[y * BOARD_SIZE + x])
                    {
                        uint8_t captures{0};
                        
                        place_stone_on_board(x, y, is_black, &captures);
                        max_h = std::max(max_h, minimax(depth-1, alpha, beta, false, !is_black));
                        remove_stone_from_board(x, y, is_black, &captures);
                        if (max_h <= *beta) {
                            prunedCount++;
                            return max_h;
                        }
                        *alpha = std::max(*alpha, max_h);
                    }
        return max_h;
    }
    else
    {
        int32_t min_h = std::numeric_limits<int32_t>::max();

        for (uint16_t y{0}; y < BOARD_SIZE; ++y)
            for (uint16_t x{0}; x < BOARD_SIZE; ++x)
                if (get_stone(x, y) == EMPTY_STONE)
                    if (move_map[y * BOARD_SIZE + x])
                    {
                        uint8_t captures{0};

                        place_stone_on_board(x, y, is_black, &captures);
                        min_h = std::min(min_h, minimax(depth-1, alpha, beta, true, !is_black));
                        remove_stone_from_board(x, y, is_black, &captures);
                        if (min_h >= *alpha)
                        {
                            prunedCount++;
                            return min_h;
                        }
                        *beta = std::max(*beta, min_h);
                    }
        return min_h;
    }
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
