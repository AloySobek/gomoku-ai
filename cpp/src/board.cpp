#include "board.hpp"
#include "Patterns.hpp"
#include <cstdlib>
#include <limits>
#include <ostream>

Board::Board()
{
    reset();
    fill_zobrist_table();
}

bool Board::place_stone_on_board(int8_t x, int8_t y, bool is_black, uint8_t *captures)
{
    if (is_black)
    {
        if (y+1 < BOARD_SIZE && y-2 >= 0
                && (white_board[y-2] & black_board[y-1] & white_board[y+1] & (0x40000 >> x)))
            return (false);
        if (y+1 < BOARD_SIZE && y-2 >= 0 && x+2 < BOARD_SIZE && x-1 >= 0
                && ((white_board [y-2] << 2) & (black_board[y-1] << 1) & (white_board [y+1] >> 1) & (0x40000 >> x)))
            return (false);
        if (x+2 < BOARD_SIZE && x-1 >= 0
                && ((white_board[y] << 2) & (black_board[y] << 1) & (white_board[y] >> 1) & (0x40000 >> x)))
            return (false);
        if (y+2 < BOARD_SIZE && y-1 >= 0 && x+2 > BOARD_SIZE && x-1 >= 0
                && ((white_board[y+2] << 2) & (black_board[y+1] << 1) & (white_board[y-1] >> 1) & (0x40000 >> x)))
            return (false);
        if (y+2 < BOARD_SIZE && y-1 >= 0
                && (white_board[y+2] & black_board[y+1] & white_board[y-1] & (0x40000 >> x)))
            return (false);
        if (y+2 < BOARD_SIZE && y-1 >= 0 && x+1 < BOARD_SIZE && x-2 >= 0
                && ((white_board[y+2] >> 2) & (black_board[y+1] >> 1) & (white_board[y-1] << 1) & (0x40000 >> x)))
            return (false);
        if (x+1 < BOARD_SIZE && x-2 >= 0
                && ((white_board[y] >> 2) & (black_board[y] >> 1) & (white_board[y] << 1) & (0x40000 >> x)))
            return (false);
        if (y+1 < BOARD_SIZE && y-2 >= 0 && x+1 < BOARD_SIZE && x-2 >= 0
                && ((white_board[y-2] >> 2) & (black_board[y-1] >> 1) & (white_board[y+1] << 1) & (0x40000 >> x)))
            return (false);
        black_board[y] |= 0x40000 >> x;
        hash ^= zobrist_table[y * BOARD_SIZE + x];
        if (captures)
        {
            if (y-3 >= 0 && (black_board[y-3] & white_board[y-2] & white_board[y-1] & (0x40000 >> x)))
                remove_stone_from_board(x,y-1,false), remove_stone_from_board(x,y-2,false), *captures |= 0x1, ++black_captures_count;
            if (y-3 >= 0 && x+3 < BOARD_SIZE && ((black_board[y-3] << 3) & (white_board[y-2] << 2) & (white_board[y-1] << 1) & (0x40000 >> x)))
                remove_stone_from_board(x+1,y-1,false), remove_stone_from_board(x+2,y-2,false), *captures |= 0x2, ++black_captures_count;
            if (x+3 < BOARD_SIZE && ((black_board[y] << 3) & (white_board[y] << 2) & (white_board[y] << 1) & (0x40000 >> x)))
                remove_stone_from_board(x+1,y,false), remove_stone_from_board(x+2,y,false), *captures |= 0x4, ++black_captures_count;
            if (y+3 < BOARD_SIZE && x+3 < BOARD_SIZE && ((black_board[y+3] << 3) & (white_board[y+2] << 2) & (white_board[y+1] << 1) & (0x40000 >> x)))
                remove_stone_from_board(x+1,y+1,false), remove_stone_from_board(x+2,y+2,false), *captures |= 0x8, ++black_captures_count;
            if (y+3 < BOARD_SIZE && (black_board[y+3] & white_board[y+2] & white_board[y+1] & (0x40000 >> x)))
                remove_stone_from_board(x,y+1,false), remove_stone_from_board(x,y+2,false), *captures |= 0x10, ++black_captures_count;
            if (y+3 < BOARD_SIZE && x-3 >= 0 && ((black_board[y+3] >> 3) & (white_board[y+2] >> 2) & (white_board[y+1] >> 1) & (0x40000 >> x)))
                remove_stone_from_board(x-1,y+1,false), remove_stone_from_board(x-2,y+2,false), *captures |= 0x20, ++black_captures_count;
            if (x-3 >= 0 && ((black_board[y] >> 3) & (white_board[y] >> 2) & (white_board[y] >> 1) & (0x40000 >> x)))
                remove_stone_from_board(x-1,y,false), remove_stone_from_board(x-2,y,false), *captures |= 0x40, ++black_captures_count;
            if (y-3 >= 0 && x-3 >= 0 && ((black_board[y-3] >> 3) & (white_board[y-2] >> 2) & (white_board[y-1] >> 1) & (0x40000 >> x)))
                remove_stone_from_board(x-1,y-1,false), remove_stone_from_board(x-2,y-2,false), *captures |= 0x80, ++black_captures_count;
        }
    }
    else
    {
        if (y+1 < BOARD_SIZE && y-2 >= 0
                && (black_board[y-2] & white_board[y-1] & black_board[y+1] & (0x40000 >> x)))
            return (false);
        if (y+1 < BOARD_SIZE && y-2 >= 0 && x+2 < BOARD_SIZE && x-1 >= 0
                && ((black_board [y-2] << 2) & (white_board[y-1] << 1) & (black_board [y+1] >> 1) & (0x40000 >> x)))
            return (false);
        if (x+2 < BOARD_SIZE && x-1 >= 0
                && ((black_board[y] << 2) & (white_board[y] << 1) & (black_board[y] >> 1) & (0x40000 >> x)))
            return (false);
        if (y+2 < BOARD_SIZE && y-1 >= 0 && x+2 > BOARD_SIZE && x-1 >= 0
                && ((black_board[y+2] << 2) & (white_board[y+1] << 1) & (black_board[y-1] >> 1) & (0x40000 >> x)))
            return (false);
        if (y+2 < BOARD_SIZE && y-1 >= 0
                && (black_board[y+2] & white_board[y+1] & black_board[y-1] & (0x40000 >> x)))
            return (false);
        if (y+2 < BOARD_SIZE && y-1 >= 0 && x+1 < BOARD_SIZE && x-2 >= 0
                && ((black_board[y+2] >> 2) & (white_board[y+1] >> 1) & (black_board[y-1] << 1) & (0x40000 >> x)))
            return (false);
        if (x+1 < BOARD_SIZE && x-2 >= 0
                && ((black_board[y] >> 2) & (white_board[y] >> 1) & (black_board[y] << 1) & (0x40000 >> x)))
            return (false);
        if (y+1 < BOARD_SIZE && y-2 >= 0 && x+1 < BOARD_SIZE && x-2 >= 0
                && ((black_board[y-2] >> 2) & (white_board[y-1] >> 1) & (black_board[y+1] << 1) & (0x40000 >> x)))
            return (false);
        white_board[y] |= 0x40000 >> x;
        hash ^= zobrist_table[(2 + y) * BOARD_SIZE + x];
        if (captures)
        {
            if (y-3 >= 0 && (white_board[y-3] & black_board[y-2] & black_board[y-1] & (0x40000 >> x)))
                remove_stone_from_board(x,y-1,true), remove_stone_from_board(x,y-2,true), *captures |= 0x1, ++white_captures_count;
            if (y-3 >= 0 && x+3 < BOARD_SIZE && ((white_board[y-3] << 3) & (black_board[y-2] << 2) & (black_board[y-1] << 1) & (0x40000 >> x)))
                remove_stone_from_board(x+1,y-1,true), remove_stone_from_board(x+2,y-2,true), *captures |= 0x2, ++white_captures_count;
            if (x+3 < BOARD_SIZE && ((white_board[y] << 3) & (black_board[y] << 2) & (black_board[y] << 1) & (0x40000 >> x)))
                remove_stone_from_board(x+1,y,true), remove_stone_from_board(x+2,y,true), *captures |= 0x4, ++white_captures_count;
            if (y+3 < BOARD_SIZE && x+3 < BOARD_SIZE && ((white_board[y+3] << 3) & (black_board[y+2] << 2) & (black_board[y+1] << 1) & (0x40000 >> x)))
                remove_stone_from_board(x+1,y+1,true), remove_stone_from_board(x+2,y+2,true), *captures |= 0x8, ++white_captures_count;
            if (y+3 < BOARD_SIZE && (white_board[y+3] & black_board[y+2] & black_board[y+1] & (0x40000 >> x)))
                remove_stone_from_board(x,y+1,true), remove_stone_from_board(x,y+2,true), *captures |= 0x10, ++white_captures_count;
            if (y+3 < BOARD_SIZE && x-3 >= 0 && ((white_board[y+3] >> 3) & (black_board[y+2] >> 2) & (black_board[y+1] >> 1) & (0x40000 >> x)))
                remove_stone_from_board(x-1,y+1,true), remove_stone_from_board(x-2,y+2,true), *captures |= 0x20, ++white_captures_count;
            if (x-3 >= 0 && ((white_board[y] >> 3) & (black_board[y] >> 2) & (black_board[y] >> 1) & (0x40000 >> x)))
                remove_stone_from_board(x-1,y,true), remove_stone_from_board(x-2,y,true), *captures |= 0x40, ++white_captures_count;
            if (y-3 >= 0 && x-3 >= 0 && ((white_board[y-3] >> 3) & (black_board[y-2] >> 2) & (black_board[y-1] >> 1) & (0x40000 >> x)))
                remove_stone_from_board(x-1,y-1,true), remove_stone_from_board(x-2,y-2,true), *captures |= 0x80, ++white_captures_count;
        }
    }
    if (y-1 >= 0)
        ++move_map[(y-1) * BOARD_SIZE + x];
    if (y-1 >= 0 && x+1 < BOARD_SIZE)
        ++move_map[(y-1) * BOARD_SIZE + (x+1)];
    if (x+1 < BOARD_SIZE)
        ++move_map[y * BOARD_SIZE + (x+1)];
    if (y+1 < BOARD_SIZE && x+1 < BOARD_SIZE)
        ++move_map[(y+1) * BOARD_SIZE + (x+1)];
    if (y+1 < BOARD_SIZE)
        ++move_map[(y+1) * BOARD_SIZE + x];
    if (y+1 < BOARD_SIZE && x-1 >= 0)
        ++move_map[(y+1) * BOARD_SIZE + (x-1)];
    if (x-1 >= 0)
        ++move_map[y * BOARD_SIZE + (x-1)];
    if (y-1 >= 0 && x-1 >= 0)
        ++move_map[(y-1) * BOARD_SIZE + (x-1)];
    return (true);
}

bool Board::remove_stone_from_board(int8_t x, int8_t y, bool is_black, uint8_t *captures)
{
    if (is_black)
    {
        black_board[y] &= ~(0x40000 >> x);
        hash ^= zobrist_table[y * BOARD_SIZE + x];
        if (captures)
        {
            if (*captures & 0x1)
                place_stone_on_board(x,y-1,!is_black), place_stone_on_board(x,y-2,!is_black), --black_captures_count;
            if (*captures & 0x2)
                place_stone_on_board(x+1,y-1,!is_black), place_stone_on_board(x+2,y-2,!is_black), --black_captures_count;
            if (*captures & 0x4)
                place_stone_on_board(x+1,y,!is_black), place_stone_on_board(x+2,y,!is_black), --black_captures_count;
            if (*captures & 0x8)
                place_stone_on_board(x+1,y+1,!is_black), place_stone_on_board(x+2,y+2,!is_black), --black_captures_count;
            if (*captures & 0x10)
                place_stone_on_board(x,y+1,!is_black), place_stone_on_board(x,y+2,!is_black), --black_captures_count;
            if (*captures & 0x20)
                place_stone_on_board(x-1,y+1,!is_black), place_stone_on_board(x-2,y+2,!is_black), --black_captures_count;
            if (*captures & 0x40)
                place_stone_on_board(x-1,y,!is_black), place_stone_on_board(x-2,y,!is_black), --black_captures_count;
            if (*captures & 0x80)
                place_stone_on_board(x-1,y-1,!is_black), place_stone_on_board(x-2,y-2,!is_black), --black_captures_count;
        }
    }
    else
    {
        white_board[y] &= ~(0x40000 >> x);
        hash ^= zobrist_table[(2 + y) * BOARD_SIZE + x];
        if (captures)
        {
            if (*captures & 0x1)
                place_stone_on_board(x,y-1,!is_black), place_stone_on_board(x,y-2,!is_black), --white_captures_count;
            if (*captures & 0x2)
                place_stone_on_board(x+1,y-1,!is_black), place_stone_on_board(x+2,y-2,!is_black), --white_captures_count;
            if (*captures & 0x4)
                place_stone_on_board(x+1,y,!is_black), place_stone_on_board(x+2,y,!is_black), --white_captures_count;
            if (*captures & 0x8)
                place_stone_on_board(x+1,y+1,!is_black), place_stone_on_board(x+2,y+2,!is_black), --white_captures_count;
            if (*captures & 0x10)
                place_stone_on_board(x,y+1,!is_black), place_stone_on_board(x,y+2,!is_black), --white_captures_count;
            if (*captures & 0x20)
                place_stone_on_board(x-1,y+1,!is_black), place_stone_on_board(x-2,y+2,!is_black), --white_captures_count;
            if (*captures & 0x40)
                place_stone_on_board(x-1,y,!is_black), place_stone_on_board(x-2,y,!is_black), --white_captures_count;
            if (*captures & 0x80)
                place_stone_on_board(x-1,y-1,!is_black), place_stone_on_board(x-2,y-2,!is_black), --white_captures_count;
        }
    }
    if (y-1 >= 0)
        --move_map[(y-1) * BOARD_SIZE + x];
    if (y-1 >= 0 && x+1 < BOARD_SIZE)
        --move_map[(y-1) * BOARD_SIZE + (x+1)];
    if (x+1 < BOARD_SIZE)
        --move_map[y * BOARD_SIZE + (x+1)];
    if (y+1 < BOARD_SIZE && x+1 < BOARD_SIZE)
        --move_map[(y+1) * BOARD_SIZE + (x+1)];
    if (y+1 < BOARD_SIZE)
        --move_map[(y+1) * BOARD_SIZE + x];
    if (y+1 < BOARD_SIZE && x-1 >= 0)
        --move_map[(y+1) * BOARD_SIZE + (x-1)];
    if (x-1 >= 0)
        --move_map[y * BOARD_SIZE + (x-1)];
    if (y-1 >= 0 && x-1 >= 0)
        --move_map[(y-1) * BOARD_SIZE + (x-1)];
    return (true);
}

int32_t Board::minimax(int8_t depth, int32_t alpha, int32_t beta, int8_t x, int8_t y, bool maximizer, bool is_black)
{
    if ((depth == 0 && ++nodes_count) || five_in_a_row(x, y, is_black))
    {
        int32_t score{0}, black_score{0}, white_score{0};

        if (hash_map.find(hash) != hash_map.end() && ++cache_hit_count)
            score = hash_map[hash];
        else
        {
            for (uint16_t y{0}; y < BOARD_SIZE; ++y)
                for (uint16_t x{0}; x < BOARD_SIZE; ++x)
                    if (move_map[y * BOARD_SIZE + x])
                    {
                        black_score += five_in_a_row(x,y,true) ? 100000 : 0;
                        black_score += open_four(x,y,true) ? 15000 : 0;
                        black_score += simple_four(x,y,true) ? 10000 : 0;
                        black_score += open_three(x,y,true) ? 10000 : 0;
                        black_score += simple_three(x,y,true) ? 500 : 0;
                        black_score += open_two(x,y,true) ? 100 : 0;
                        black_score += simple_two(x,y,true) ? 50 : 0; 

                        white_score += five_in_a_row(x,y,false) ? 100000 : 0;
                        white_score += open_four(x,y,false) ? 15000 : 0;
                        white_score += simple_four(x,y,false) ? 10000 : 0;
                        white_score += open_three(x,y,false) ? 10000 : 0;
                        white_score += simple_three(x,y,false) ? 500 : 0;
                        white_score += open_two(x,y,false) ? 100 : 0;
                        white_score += simple_two(x,y,false) ? 50 : 0;
                    }
            if ((is_black && maximizer) || (!is_black && !maximizer))
                score = black_score - white_score;
            else
                score = white_score - black_score;
            hash_map[hash] = score;
        }
        return (score);
    }
    if (maximizer)
    {
        int32_t max_h = std::numeric_limits<int32_t>::min();
        for (uint16_t y{0}; y < BOARD_SIZE; ++y)
            for (uint16_t x{0}; x < BOARD_SIZE; ++x)
                if (move_map[y * BOARD_SIZE + x])
                    if (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))
                    {
                        uint8_t captures{0};
                        place_stone_on_board(x, y, is_black, &captures);
                        max_h = std::max(max_h, minimax(depth-1, alpha, beta,x,y, false, !is_black));
                        remove_stone_from_board(x, y, is_black, &captures);
                        alpha = std::max(alpha, max_h);
                        if (beta <= alpha && ++pruned_count)
                            return (max_h);
                    }
        return (max_h);
    }
    else
    {
        int32_t min_h = std::numeric_limits<int32_t>::max();
        for (uint16_t y{0}; y < BOARD_SIZE; ++y)
            for (uint16_t x{0}; x < BOARD_SIZE; ++x)
                if (move_map[y * BOARD_SIZE + x])
                    if (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))
                    {
                        uint8_t captures{0};
                        place_stone_on_board(x, y, is_black, &captures);
                        min_h = std::min(min_h, minimax(depth-1, alpha, beta, true,x,y, !is_black));
                        remove_stone_from_board(x, y, is_black, &captures);
                        beta = std::min(beta, min_h);
                        if (beta <= alpha && ++pruned_count)
                            return (min_h);
                    }
        return (min_h);
    }
}

int32_t Board::ai_move(bool is_black)
{
    int32_t max_h{std::numeric_limits<int32_t>::min()};
    int32_t move{0};
    int32_t h{0};

    cache_hit_count = 0;
    pruned_count = 0;
    nodes_count = 0;

    hash = get_hash();
    hash_map.clear();

    for (uint16_t y{0}; y < BOARD_SIZE; ++y)
        for (uint8_t x{0}; x < BOARD_SIZE; ++x)
            if (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))
                if (move_map[y * BOARD_SIZE + x])
                {
                    uint8_t captures{0};
                    place_stone_on_board(x, y, is_black, &captures);
                    h = minimax(3, std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max(), x, y, true, is_black);
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

void Board::fill_zobrist_table()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<uint64_t> uni(0,std::numeric_limits<uint64_t>::max());

    for (int8_t z{0}; z < 2; ++z)
        for (int8_t y{0}; y < BOARD_SIZE; ++y)
            for (int8_t x{0}; x < BOARD_SIZE; ++x)
            {
                auto r = uni(rng);
                zobrist_table[(z * 2 + y) * BOARD_SIZE + x] = r;
            }
}

uint64_t Board::get_hash()
{
    uint64_t hash{0};

    for (int8_t y{0}; y < BOARD_SIZE; ++y)
        for (uint8_t x{0}; x < BOARD_SIZE; ++x)
        {
            if (black_board[y] & (0x40000 >> x))
                hash ^= zobrist_table[y * BOARD_SIZE +x];
            if (white_board[y] & (0x40000 >> x))
                hash ^= zobrist_table[(2 + y) * BOARD_SIZE + x];
        }
    return (hash);
}

bool Board::five_in_a_row(int8_t x, int8_t y, bool is_black)
{
    if (is_black && (black_board[y] & (0x40000 >> x))) // 11111
    {
        if (y+4 < BOARD_SIZE && (black_board[y] & black_board[y+1] & black_board[y+2] & black_board[y+3] & black_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && (black_board[y] & (black_board[y] << 1) & (black_board[y] << 2) & (black_board[y] << 3) & (black_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && (black_board[y] & (black_board[y+1] << 1) & (black_board[y+2] << 2) & (black_board[y+3] << 3) & (black_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && (black_board[y] & (black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (black_board[y+3] >> 3) & (black_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (white_board[y] & (0x40000 >> x))) // 22222
    {
        if (y+4 < BOARD_SIZE && (black_board[y] & black_board[y+1] & black_board[y+2] & black_board[y+3] & black_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && (black_board[y] & (black_board[y] << 1) & (black_board[y] << 2) & (black_board[y] << 3) & (black_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && (black_board[y] & (black_board[y+1] << 1) & (black_board[y+2] << 2) & (black_board[y+3] << 3) & (black_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && (black_board[y] & (black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (black_board[y+3] >> 3) & (black_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);
    }
    return (false);
}

bool Board::open_four(int8_t x, int8_t y, bool is_black)
{
    if (is_black && (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))) // .1111.
    {
        if (y+5 < BOARD_SIZE && !(black_board[y+5] & (0x40000 >> x)) && !(white_board[y+5] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+2] & black_board[y+3] & black_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+5))) && !(white_board[y] & (0x40000 >> (x+5)))
                && ((black_board[y] << 1) & (black_board[y] << 2) & (black_board[y] << 3) & (black_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && y+5 < BOARD_SIZE && !(black_board[y+5] & (0x40000 >> (x+5))) && !(white_board[y+5] & (0x40000 >> (x+5)))
                && ((black_board[y+1] << 1) & (black_board[y+2] << 2) & (black_board[y+3] << 3) & (black_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-5 < BOARD_SIZE && y+5 < BOARD_SIZE && !(black_board[y+5] & (0x40000 >> (x-5))) && !(white_board[y+5] & (0x40000 >> (x-5)))
                && ((black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (black_board[y+3] >> 3) & (black_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))) // .2222.
    {
        if (y+5 < BOARD_SIZE && !(white_board[y+5] & (0x40000 >> x)) && !(black_board[y+5] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+2] & white_board[y+3] & white_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+5))) && !(black_board[y] & (0x40000 >> (x+5)))
                && ((white_board[y] << 1) & (white_board[y] << 2) & (white_board[y] << 3) & (white_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && y+5 < BOARD_SIZE && !(white_board[y+5] & (0x40000 >> (x+5))) && !(black_board[y+5] & (0x40000 >> (x+5)))
                && ((white_board[y+1] << 1) & (white_board[y+2] << 2) & (white_board[y+3] << 3) & (white_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-5 < BOARD_SIZE && y+5 < BOARD_SIZE && !(white_board[y+5] & (0x40000 >> (x-5))) && !(black_board[y+5] & (0x40000 >> (x-5)))
                && ((white_board[y+1] >> 1) & (white_board[y+2] >> 2) & (white_board[y+3] >> 3) & (white_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);
    }
    if (is_black && (black_board[y] & (0x40000 >> x)))
    {
        // 11.11.11
        if (y+7 < BOARD_SIZE && !(black_board[y+2] & (0x40000 >> x)) && !(white_board[y+2] & (0x40000 >> x))
                && !(black_board[y+5] & (0x40000 >> x)) && !(white_board[y+5] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+3] & black_board[y+4] & black_board[y+6] & black_board[y+7] & (0x40000 >> x)))
            return (true);
        if (x+7 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+2))) && !(white_board[y] & (0x40000 >> (x+2)))
                && !(black_board[y] & (0x40000 >> (x+5))) && !(white_board[y] & (0x40000 >> (x+5)))
                && ((black_board[y] << 1) & (black_board[y] << 3) & (black_board[y] << 4) & (black_board[y] << 6) & (black_board[y] << 7) & (0x40000 >> x)))
            return (true);
        if (x+7 < BOARD_SIZE && y+7 < BOARD_SIZE && !(black_board[y+2] & (0x40000 >> (x+2))) && !(white_board[y+2] & (0x40000 >> (x+2)))
                && !(black_board[y+5] & (0x40000 >> (x+5))) && !(white_board[y+5] & (0x40000 >> (x+5)))
                && ((black_board[y+1] << 1) & (black_board[y+3] << 3) & (black_board[y+4] << 4) & (black_board[y+6] << 6) & (black_board[y+7] << 7) & (0x40000 >> x)))
            return (true);
        if (x-7 < BOARD_SIZE && y+7 < BOARD_SIZE && !(black_board[y+2] & (0x40000 >> (x-2))) && !(white_board[y+2] & (0x40000 >> (x-2)))
                && !(black_board[y+5] & (0x40000 >> (x-5))) && !(white_board[y+5] & (0x40000 >> (x-5)))
                && ((black_board[y+1] >> 1) & (black_board[y+3] >> 3) & (black_board[y+4] >> 4) & (black_board[y+6] >> 6) & (black_board[y+7] >> 7) & (0x40000 >> x)))
            return (true);

        // 111.1.111
        if (y+8 < BOARD_SIZE && !(black_board[y+3] & (0x40000 >> x)) && !(white_board[y+3] & (0x40000 >> x))
                && !(black_board[y+5] & (0x40000 >> x)) && !(white_board[y+5] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+2] & black_board[y+4] & black_board[y+6] & black_board[y+7] & black_board[y+8] & (0x40000 >> x)))
            return (true);
        if (x+8 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+3))) && !(white_board[y] & (0x40000 >> (x+3)))
                && !(black_board[y] & (0x40000 >> (x+5))) && !(white_board[y] & (0x40000 >> (x+5)))
                && ((black_board[y] << 1) & (black_board[y] << 2) & (black_board[y] << 4) & (black_board[y] << 6) & (black_board[y] << 7) & (black_board[y] << 8) & (0x40000 >> x)))
            return (true);
        if (x+8 < BOARD_SIZE && y+8 < BOARD_SIZE && !(black_board[y+3] & (0x40000 >> (x+3))) && !(white_board[y+3] & (0x40000 >> (x+3)))
                && !(black_board[y+5] & (0x40000 >> (x+5))) && !(white_board[y+5] & (0x40000 >> (x+5)))
                && ((black_board[y+1] << 1) & (black_board[y+2] << 2) & (black_board[y+4] << 4) & (black_board[y+6] << 6) & (black_board[y+7] << 7) & (black_board[y+8] << 8) & (0x40000 >> x)))
            return (true);
        if (x-8 < BOARD_SIZE && y+8 < BOARD_SIZE && !(black_board[y+3] & (0x40000 >> (x-3))) && !(white_board[y+3] & (0x40000 >> (x-3)))
                && !(black_board[y+5] & (0x40000 >> (x-5))) && !(white_board[y+5] & (0x40000 >> (x-5)))
                && ((black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (black_board[y+4] >> 4) & (black_board[y+6] >> 6) & (black_board[y+7] >> 7) & (black_board[y+8] >> 8) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (white_board[y] & (0x40000 >> x)))
    {
        // 22.22.22
        if (y+7 < BOARD_SIZE && !(white_board[y+2] & (0x40000 >> x)) && !(black_board[y+2] & (0x40000 >> x))
                && !(white_board[y+5] & (0x40000 >> x)) && !(black_board[y+5] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+3] & white_board[y+4] & white_board[y+6] & white_board[y+7] & (0x40000 >> x)))
            return (true);
        if (x+7 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+2))) && !(black_board[y] & (0x40000 >> (x+2)))
                && !(white_board[y] & (0x40000 >> (x+5))) && !(black_board[y] & (0x40000 >> (x+5)))
                && ((white_board[y] << 1) & (white_board[y] << 3) & (white_board[y] << 4) & (white_board[y] << 6) & (white_board[y] << 7) & (0x40000 >> x)))
            return (true);
        if (x+7 < BOARD_SIZE && y+7 < BOARD_SIZE && !(white_board[y+2] & (0x40000 >> (x+2))) && !(black_board[y+2] & (0x40000 >> (x+2)))
                && !(white_board[y+5] & (0x40000 >> (x+5))) && !(black_board[y+5] & (0x40000 >> (x+5)))
                && ((white_board[y+1] << 1) & (white_board[y+3] << 3) & (white_board[y+4] << 4) & (white_board[y+6] << 6) & (white_board[y+7] << 7) & (0x40000 >> x)))
            return (true);
        if (x-7 < BOARD_SIZE && y+7 < BOARD_SIZE && !(white_board[y+2] & (0x40000 >> (x-2))) && !(black_board[y+2] & (0x40000 >> (x-2)))
                && !(white_board[y+5] & (0x40000 >> (x-5))) && !(black_board[y+5] & (0x40000 >> (x-5)))
                && ((white_board[y+1] >> 1) & (white_board[y+3] >> 3) & (white_board[y+4] >> 4) & (white_board[y+6] >> 6) & (white_board[y+7] >> 7) & (0x40000 >> x)))
            return (true);

        // 222.2.222
        if (y+8 < BOARD_SIZE && !(white_board[y+3] & (0x40000 >> x)) && !(black_board[y+3] & (0x40000 >> x))
                && !(white_board[y+5] & (0x40000 >> x)) && !(black_board[y+5] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+2] & white_board[y+4] & white_board[y+6] & white_board[y+7] & white_board[y+8] & (0x40000 >> x)))
            return (true);
        if (x+8 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+3))) && !(black_board[y] & (0x40000 >> (x+3)))
                && !(white_board[y] & (0x40000 >> (x+5))) && !(black_board[y] & (0x40000 >> (x+5)))
                && ((white_board[y] << 1) & (white_board[y] << 2) & (white_board[y] << 4) & (white_board[y] << 6) & (white_board[y] << 7) & (white_board[y] << 8) & (0x40000 >> x)))
            return (true);
        if (x+8 < BOARD_SIZE && y+8 < BOARD_SIZE && !(white_board[y+3] & (0x40000 >> (x+3))) && !(black_board[y+3] & (0x40000 >> (x+3)))
                && !(white_board[y+5] & (0x40000 >> (x+5))) && !(black_board[y+5] & (0x40000 >> (x+5)))
                && ((white_board[y+1] << 1) & (white_board[y+2] << 2) & (white_board[y+4] << 4) & (white_board[y+6] << 6) & (white_board[y+7] << 7) & (white_board[y+8] << 8) & (0x40000 >> x)))
            return (true);
        if (x-8 < BOARD_SIZE && y+8 < BOARD_SIZE && !(white_board[y+3] & (0x40000 >> (x-3))) && !(black_board[y+3] & (0x40000 >> (x-3)))
                && !(white_board[y+5] & (0x40000 >> (x-5))) && !(black_board[y+5] & (0x40000 >> (x-5)))
                && ((white_board[y+1] >> 1) & (white_board[y+2] >> 2) & (white_board[y+4] >> 4) & (white_board[y+6] >> 6) & (white_board[y+7] >> 7) & (white_board[y+8] >> 8) & (0x40000 >> x)))
            return (true);
    }
    return (false);
}

bool Board::simple_four(int8_t x, int8_t y, bool is_black)
{
    if (is_black && (white_board[y] & (0x40000 >> x))) // 21111.
    {
        if (y+5 < BOARD_SIZE && !(black_board[y+5] & (0x40000 >> x)) && !(white_board[y+5] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+2] & black_board[y+3] & black_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+5))) && !(white_board[y] & (0x40000 >> (x+5)))
                && ((black_board[y] << 1) & (black_board[y] << 2) & (black_board[y] << 3) & (black_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && y+5 < BOARD_SIZE && !(black_board[y+5] & (0x40000 >> (x+5))) && !(white_board[y+5] & (0x40000 >> (x+5)))
                && ((black_board[y+1] << 1) & (black_board[y+2] << 2) & (black_board[y+3] << 3) & (black_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-5 < BOARD_SIZE && y+5 < BOARD_SIZE && !(black_board[y+5] & (0x40000 >> (x-5))) && !(white_board[y+5] & (0x40000 >> (x-5)))
                && ((black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (black_board[y+3] >> 3) & (black_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);
    }
    else if (is_black && (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))) // .11112
    {
        if (y+5 < BOARD_SIZE && (white_board[y+5] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+2] & black_board[y+3] & black_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && (white_board[y] & (0x40000 >> (x+5)))
                && ((black_board[y] << 1) & (black_board[y] << 2) & (black_board[y] << 3) & (black_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && y+5 < BOARD_SIZE && (white_board[y+5] & (0x40000 >> (x+5)))
                && ((black_board[y+1] << 1) & (black_board[y+2] << 2) & (black_board[y+3] << 3) & (black_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-5 < BOARD_SIZE && y+5 < BOARD_SIZE && (white_board[y+5] & (0x40000 >> (x-5)))
                && ((black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (black_board[y+3] >> 3) & (black_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (black_board[y] & (0x40000 >> x))) // 12222.
    {
        if (y+5 < BOARD_SIZE && !(white_board[y+5] & (0x40000 >> x)) && !(black_board[y+5] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+2] & white_board[y+3] & white_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+5))) && !(black_board[y] & (0x40000 >> (x+5)))
                && ((white_board[y] << 1) & (white_board[y] << 2) & (white_board[y] << 3) & (white_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && y+5 < BOARD_SIZE && !(white_board[y+5] & (0x40000 >> (x+5))) && !(black_board[y+5] & (0x40000 >> (x+5)))
                && ((white_board[y+1] << 1) & (white_board[y+2] << 2) & (white_board[y+3] << 3) & (white_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-5 < BOARD_SIZE && y+5 < BOARD_SIZE && !(white_board[y+5] & (0x40000 >> (x-5))) && !(black_board[y+5] & (0x40000 >> (x-5)))
                && ((white_board[y+1] >> 1) & (white_board[y+2] >> 2) & (white_board[y+3] >> 3) & (white_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))) // .22221
    {
        if (y+5 < BOARD_SIZE && (black_board[y+5] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+2] & white_board[y+3] & white_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && (black_board[y] & (0x40000 >> (x+5)))
                && ((white_board[y] << 1) & (white_board[y] << 2) & (white_board[y] << 3) & (white_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+5 < BOARD_SIZE && y+5 < BOARD_SIZE && (black_board[y+5] & (0x40000 >> (x+5)))
                && ((white_board[y+1] << 1) & (white_board[y+2] << 2) & (white_board[y+3] << 3) & (white_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-5 < BOARD_SIZE && y+5 < BOARD_SIZE && (black_board[y+5] & (0x40000 >> (x-5)))
                && ((white_board[y+1] >> 1) & (white_board[y+2] >> 2) & (white_board[y+3] >> 3) & (white_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);
    }

    if (is_black && (black_board[y] & (0x40000 >> x)))
    {
        // 1.111
        if (y+4 < BOARD_SIZE && !(black_board[y+1] & (0x40000 >> x)) && !(white_board[y+1] & (0x40000 >> x))
                && (black_board[y+2] & black_board[y+3] & black_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+1))) && !(white_board[y] & (0x40000 >> (x+1)))
                && ((black_board[y] << 2) & (black_board[y] << 3) & (black_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(black_board[y+1] & (0x40000 >> (x+1))) && !(white_board[y+1] & (0x40000 >> (x+1)))
                && ((black_board[y+2] << 2) & (black_board[y+3] << 3) & (black_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(black_board[y+1] & (0x40000 >> (x-1))) && !(white_board[y+1] & (0x40000 >> (x-1)))
                && ((black_board[y+2] >> 2) & (black_board[y+3] >> 3) & (black_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);

        // 111.1
        if (y+4 < BOARD_SIZE && !(black_board[y+3] & (0x40000 >> x)) && !(white_board[y+3] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+2] & black_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+3))) && !(white_board[y] & (0x40000 >> (x+3)))
                && ((black_board[y] << 1) & (black_board[y] << 2) & (black_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(black_board[y+3] & (0x40000 >> (x+3))) && !(white_board[y+3] & (0x40000 >> (x+3)))
                && ((black_board[y+1] << 1) & (black_board[y+2] << 2) & (black_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(black_board[y+3] & (0x40000 >> (x-3))) && !(white_board[y+3] & (0x40000 >> (x-3)))
                && ((black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (black_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (white_board[y] & (0x40000 >> x)))
    {
        // 2.222
        if (y+4 < BOARD_SIZE && !(white_board[y+1] & (0x40000 >> x)) && !(black_board[y+1] & (0x40000 >> x))
                && (white_board[y+2] & white_board[y+3] & white_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+1))) && !(black_board[y] & (0x40000 >> (x+1)))
                && ((white_board[y] << 2) & (white_board[y] << 3) & (white_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(white_board[y+1] & (0x40000 >> (x+1))) && !(black_board[y+1] & (0x40000 >> (x+1)))
                && ((white_board[y+2] << 2) & (white_board[y+3] << 3) & (white_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(white_board[y+1] & (0x40000 >> (x-1))) && !(black_board[y+1] & (0x40000 >> (x-1)))
                && ((white_board[y+2] >> 2) & (white_board[y+3] >> 3) & (white_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);

        // 222.2
        if (y+4 < BOARD_SIZE && !(white_board[y+3] & (0x40000 >> x)) && !(black_board[y+3] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+2] & white_board[y+4] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+3))) && !(black_board[y] & (0x40000 >> (x+3)))
                && ((white_board[y] << 1) & (white_board[y] << 2) & (white_board[y] << 4) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(white_board[y+3] & (0x40000 >> (x+3))) && !(black_board[y+3] & (0x40000 >> (x+3)))
                && ((white_board[y+1] << 1) & (white_board[y+2] << 2) & (white_board[y+4] << 4) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(white_board[y+3] & (0x40000 >> (x-3))) && !(black_board[y+3] & (0x40000 >> (x-3)))
                && ((white_board[y+1] >> 1) & (white_board[y+2] >> 2) & (white_board[y+4] >> 4) & (0x40000 >> x)))
            return (true);
    }
    return (false);
}

bool Board::open_three(int8_t x, int8_t y, bool is_black)
{
    if (is_black && (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))) // .111.
    {
        if (y+4 < BOARD_SIZE && !(black_board[y+4] & (0x40000 >> x)) && !(white_board[y+4] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+2] & black_board[y+3] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+4))) && !(white_board[y] & (0x40000 >> (x+4)))
                && ((black_board[y] << 1) & (black_board[y] << 2) & (black_board[y] << 3) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(black_board[y+4] & (0x40000 >> (x+4))) && !(white_board[y+4] & (0x40000 >> (x+4)))
                && ((black_board[y+1] << 1) & (black_board[y+2] << 2) & (black_board[y+3] << 3) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(black_board[y+4] & (0x40000 >> (x-4))) && !(white_board[y+4] & (0x40000 >> (x-4)))
                && ((black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (black_board[y+3] >> 3) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))) // .222.
    {
        if (y+4 < BOARD_SIZE && !(white_board[y+4] & (0x40000 >> x)) && !(black_board[y+4] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+2] & white_board[y+3] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+4))) && !(black_board[y] & (0x40000 >> (x+4)))
                && ((white_board[y] << 1) & (white_board[y] << 2) & (white_board[y] << 3) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(white_board[y+4] & (0x40000 >> (x+4))) && !(black_board[y+4] & (0x40000 >> (x+4)))
                && ((white_board[y+1] << 1) & (white_board[y+2] << 2) & (white_board[y+3] << 3) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(white_board[y+4] & (0x40000 >> (x-4))) && !(black_board[y+4] & (0x40000 >> (x-4)))
                && ((white_board[y+1] >> 1) & (white_board[y+2] >> 2) & (white_board[y+3] >> 3) & (0x40000 >> x)))
            return (true);
    };
    return (false);
}

bool Board::simple_three(int8_t x, int8_t y, bool is_black)
{
    if (is_black && (black_board[y] & (0x40000 >> x)))
    {
        // 1.11
        if (y+3 < BOARD_SIZE && !(black_board[y+1] & (0x40000 >> x)) && !(white_board[y+1] & (0x40000 >> x))
                && (black_board[y+2] & black_board[y+3] & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+1))) && !(white_board[y] & (0x40000 >> (x+1)))
                && ((black_board[y] << 2) & (black_board[y] << 3) & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(black_board[y+1] & (0x40000 >> (x+1))) && !(white_board[y+1] & (0x40000 >> (x+1)))
                && ((black_board[y+2] << 2) & (black_board[y+3] << 3) & (0x40000 >> x)))
            return (true);
        if (x-3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(black_board[y+1] & (0x40000 >> (x-1))) && !(white_board[y+1] & (0x40000 >> (x-1)))
                && ((black_board[y+2] >> 2) & (black_board[y+3] >> 3) & (0x40000 >> x)))
            return (true);

        // 11.1
        if (y+3 < BOARD_SIZE && !(black_board[y+2] & (0x40000 >> x)) && !(white_board[y+2] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+3] & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+2))) && !(white_board[y] & (0x40000 >> (x+2)))
                && ((black_board[y] << 1) & (black_board[y] << 3) & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(black_board[y+2] & (0x40000 >> (x+2))) && !(white_board[y+2] & (0x40000 >> (x+2)))
                && ((black_board[y+1] << 1) & (black_board[y+3] << 3) & (0x40000 >> x)))
            return (true);
        if (x-3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(black_board[y+2] & (0x40000 >> (x-2))) && !(white_board[y+2] & (0x40000 >> (x-2)))
                && ((black_board[y+1] >> 1) & (black_board[y+3] >> 3) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (white_board[y] & (0x40000 >> x)))
    {
        // 2.22
        if (y+3 < BOARD_SIZE && !(white_board[y+1] & (0x40000 >> x)) && !(black_board[y+1] & (0x40000 >> x))
                && (white_board[y+2] & white_board[y+3] & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+1))) && !(black_board[y] & (0x40000 >> (x+1)))
                && ((white_board[y] << 2) & (white_board[y] << 3) & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(white_board[y+1] & (0x40000 >> (x+1))) && !(black_board[y+1] & (0x40000 >> (x+1)))
                && ((white_board[y+2] << 2) & (white_board[y+3] << 3) & (0x40000 >> x)))
            return (true);
        if (x-3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(white_board[y+1] & (0x40000 >> (x-1))) && !(black_board[y+1] & (0x40000 >> (x-1)))
                && ((white_board[y+2] >> 2) & (white_board[y+3] >> 3) & (0x40000 >> x)))
            return (true);

        // 22.2
        if (y+3 < BOARD_SIZE && !(white_board[y+2] & (0x40000 >> x)) && !(black_board[y+2] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+3] & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+2))) && !(black_board[y] & (0x40000 >> (x+2)))
                && ((white_board[y] << 1) & (white_board[y] << 3) & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(white_board[y+2] & (0x40000 >> (x+2))) && !(black_board[y+2] & (0x40000 >> (x+2)))
                && ((white_board[y+1] << 1) & (white_board[y+3] << 3) & (0x40000 >> x)))
            return (true);
        if (x-3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(white_board[y+2] & (0x40000 >> (x-2))) && !(black_board[y+2] & (0x40000 >> (x-2)))
                && ((white_board[y+1] >> 1) & (white_board[y+3] >> 3) & (0x40000 >> x)))
            return (true);
    }

    if (is_black && (white_board[y] & (0x40000 >> x))) // 2111.
    {
        if (y+4 < BOARD_SIZE && !(black_board[y+4] & (0x40000 >> x)) && !(white_board[y+4] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+2] & black_board[y+3] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+4))) && !(white_board[y] & (0x40000 >> (x+4)))
                && ((black_board[y] << 1) & (black_board[y] << 2) & (black_board[y] << 3) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(black_board[y+4] & (0x40000 >> (x+4))) && !(white_board[y+4] & (0x40000 >> (x+4)))
                && ((black_board[y+1] << 1) & (black_board[y+2] << 2) & (black_board[y+3] << 3) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(black_board[y+4] & (0x40000 >> (x-4))) && !(white_board[y+4] & (0x40000 >> (x-4)))
                && ((black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (black_board[y+3] >> 3) & (0x40000 >> x)))
            return (true);
    }
    else if (is_black && (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))) // .1112
    {
        if (y+4 < BOARD_SIZE && (white_board[y+4] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+2] & black_board[y+3] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && (white_board[y] & (0x40000 >> (x+4)))
                && ((black_board[y] << 1) & (black_board[y] << 2) & (black_board[y] << 3) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && (white_board[y+4] & (0x40000 >> (x+4)))
                && ((black_board[y+1] << 1) & (black_board[y+2] << 2) & (black_board[y+3] << 3) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && (white_board[y+4] & (0x40000 >> (x-4)))
                && ((black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (black_board[y+3] >> 3) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (black_board[y] & (0x40000 >> x))) // 1222.
    {
        if (y+4 < BOARD_SIZE && !(white_board[y+4] & (0x40000 >> x)) && !(black_board[y+4] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+2] & white_board[y+3] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+4))) && !(black_board[y] & (0x40000 >> (x+4)))
                && ((white_board[y] << 1) & (white_board[y] << 2) & (white_board[y] << 3) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(white_board[y+4] & (0x40000 >> (x+4))) && !(black_board[y+4] & (0x40000 >> (x+4)))
                && ((white_board[y+1] << 1) & (white_board[y+2] << 2) & (white_board[y+3] << 3) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && !(white_board[y+4] & (0x40000 >> (x-4))) && !(black_board[y+4] & (0x40000 >> (x-4)))
                && ((white_board[y+1] >> 1) & (white_board[y+2] >> 2) & (white_board[y+3] >> 3) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))) // .2221
    {
        if (y+4 < BOARD_SIZE && (black_board[y+4] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+2] & white_board[y+3] & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && (black_board[y] & (0x40000 >> (x+4)))
                && ((white_board[y] << 1) & (white_board[y] << 2) & (white_board[y] << 3) & (0x40000 >> x)))
            return (true);
        if (x+4 < BOARD_SIZE && y+4 < BOARD_SIZE && (black_board[y+4] & (0x40000 >> (x+4)))
                && ((white_board[y+1] << 1) & (white_board[y+2] << 2) & (white_board[y+3] << 3) & (0x40000 >> x)))
            return (true);
        if (x-4 < BOARD_SIZE && y+4 < BOARD_SIZE && (black_board[y+4] & (0x40000 >> (x-4)))
                && ((white_board[y+1] >> 1) & (white_board[y+2] >> 2) & (white_board[y+3] >> 3) & (0x40000 >> x)))
            return (true);
    }
    return (false);
}

bool Board::open_two(int8_t x, int8_t y, bool is_black)
{
    if (is_black && (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))) // .11.
    {
        if (y+3 < BOARD_SIZE && !(black_board[y+3] & (0x40000 >> x)) && !(white_board[y+3] & (0x40000 >> x))
                && (black_board[y+1] & black_board[y+2] & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && !(black_board[y] & (0x40000 >> (x+3))) && !(white_board[y] & (0x40000 >> (x+3)))
                && ((black_board[y] << 1) & (black_board[y] << 2) & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(black_board[y+3] & (0x40000 >> (x+3))) && !(white_board[y+3] & (0x40000 >> (x+3)))
                && ((black_board[y+1] << 1) & (black_board[y+2] << 2) & (0x40000 >> x)))
            return (true);
        if (x-3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(black_board[y+3] & (0x40000 >> (x-3))) && !(white_board[y+3] & (0x40000 >> (x-3)))
                && ((black_board[y+1] >> 1) & (black_board[y+2] >> 2) & (0x40000 >> x)))
            return (true);
    }
    else if (!is_black && (!(black_board[y] & (0x40000 >> x)) && !(white_board[y] & (0x40000 >> x)))) // .22.
    {
        if (y+3 < BOARD_SIZE && !(white_board[y+3] & (0x40000 >> x)) && !(black_board[y+3] & (0x40000 >> x))
                && (white_board[y+1] & white_board[y+2] & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && !(white_board[y] & (0x40000 >> (x+3))) && !(black_board[y] & (0x40000 >> (x+3)))
                && ((white_board[y] << 1) & (white_board[y] << 2) & (0x40000 >> x)))
            return (true);
        if (x+3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(white_board[y+3] & (0x40000 >> (x+3))) && !(black_board[y+3] & (0x40000 >> (x+3)))
                && ((white_board[y+1] << 1) & (white_board[y+2] << 2) & (0x40000 >> x)))
            return (true);
        if (x-3 < BOARD_SIZE && y+3 < BOARD_SIZE && !(white_board[y+3] & (0x40000 >> (x-3))) && !(black_board[y+3] & (0x40000 >> (x-3)))
                && ((white_board[y+1] >> 1) & (white_board[y+2] >> 2) & (0x40000 >> x)))
            return (true);
    }
    return (false);
}

bool Board::simple_two(int8_t x, int8_t y, bool is_black)
{
    return (false);
}
