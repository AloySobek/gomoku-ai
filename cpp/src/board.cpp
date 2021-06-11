#include "board.hpp"
#include "Patterns.hpp"
#include <cstdlib>
#include <limits>
#include <ostream>

Board::Board()
{
    fill_zobrist_table();
    reset();
    (void)0;
}

bool Board::place_stone_on_board(int8_t x, int8_t y, bool is_black, uint8_t *captures)
{
    if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
    {
        setToken(x, y, is_black ? BLACK : WHITE);
        if (is_black)
        {
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
        setToken(x, y, EMPTY);
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
        for (int16_t y2 = y-1; y2 <= y+1; ++y2)
            for (int16_t x2 = x-1; x2 <= x+1; ++x2)
                if (x2 >= 0 && x2 < BOARD_SIZE && y2 >= 0 && y2 < BOARD_SIZE)
                    --move_map[y2 * BOARD_SIZE + x2];
        
        return (true);
    }
    return (false);
}

int32_t Board::minimax(int8_t depth, int32_t alpha, int32_t beta, int8_t x, int8_t y, bool maximizer, bool is_black)
{
    ++nodes_count;

    std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now()- startTime;
    if (depth == 0 || elapsed.count() > 0.48)
    {
        move = is_black ? BLACK : WHITE;
        result = _; // TODO;
        int32_t score;
        if (hash_map.find(hash) != hash_map.end() && ++cache_hit_count)
            score = hash_map[hash];
        else
        {
//            std::clog << *this;
            score = -Eval(); // WHITE BLACK SWAPPED
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
    startTime = std::chrono::high_resolution_clock::now();

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
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            setToken(x, y, EMPTY);
        }
    }
    for (int8_t i{0}; i < BOARD_SIZE; ++i)
    {
        black_board[i] = 0;
        white_board[i] = 0;
        for (int8_t j{0}; j < BOARD_SIZE; ++j)
            move_map[i * BOARD_SIZE + j] = 0;
    }
    setToken(BOARD_SIZE / 2, BOARD_SIZE / 2, BLACK);
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

int Board::PtrMatchFree3(Board::Color color) const {

    if (color == WHITE)
    {
        return PtrGlobalMatch(rows, wFree3_2) +
               PtrGlobalMatch(columns, wFree3_2) +
               PtrGlobalMatch(up, wFree3_2, true) +
               PtrGlobalMatch(down, wFree3_2, true) +

               PtrGlobalMatch(rows, wFree3_3) +
               PtrGlobalMatch(columns, wFree3_3) +
               PtrGlobalMatch(up, wFree3_3, true) +
               PtrGlobalMatch(down, wFree3_3, true) -

               PtrGlobalMatch(rows, wFree3_1) -
               PtrGlobalMatch(columns, wFree3_1) -
               PtrGlobalMatch(up, wFree3_1, true) -
               PtrGlobalMatch(down, wFree3_1, true) +

               PtrGlobalMatch(rows, wFree3_4) +
               PtrGlobalMatch(columns, wFree3_4) +
               PtrGlobalMatch(up, wFree3_4, true) +
               PtrGlobalMatch(down, wFree3_4, true) +

               PtrGlobalMatch(rows, wFree3_5) +
               PtrGlobalMatch(columns, wFree3_5) +
               PtrGlobalMatch(up, wFree3_5, true) +
               PtrGlobalMatch(down, wFree3_5, true);
    }
    else
    {
        return PtrGlobalMatch(rows, bFree3_2) +
               PtrGlobalMatch(columns, bFree3_2) +
               PtrGlobalMatch(up, bFree3_2, true) +
               PtrGlobalMatch(down, bFree3_2, true) +

               PtrGlobalMatch(rows, bFree3_3) +
               PtrGlobalMatch(columns, bFree3_3) +
               PtrGlobalMatch(up, bFree3_3, true) +
               PtrGlobalMatch(down, bFree3_3, true)  -

               PtrGlobalMatch(rows, bFree3_1)  -
               PtrGlobalMatch(columns, bFree3_1)  -
               PtrGlobalMatch(up, bFree3_1, true)  -
               PtrGlobalMatch(down, bFree3_1, true) +

               PtrGlobalMatch(rows, bFree3_4) +
               PtrGlobalMatch(columns, bFree3_4) +
               PtrGlobalMatch(up, bFree3_4, true) +
               PtrGlobalMatch(down, bFree3_4, true) +

               PtrGlobalMatch(rows, bFree3_5) +
               PtrGlobalMatch(columns, bFree3_5) +
               PtrGlobalMatch(up, bFree3_5, true) +
               PtrGlobalMatch(down, bFree3_5, true);
    }
}

int Board::PtrMatchHalf4(Board::Color color) const {
    if (color == Color::WHITE)
    {
        return PtrGlobalMatch(rows, wHalf4_1) +
               PtrGlobalMatch(columns, wHalf4_1) +
               PtrGlobalMatch(up, wHalf4_1, true) +
               PtrGlobalMatch(down, wHalf4_1, true) +

               PtrGlobalMatch(rows, wHalf4_2) +
               PtrGlobalMatch(columns, wHalf4_2) +
               PtrGlobalMatch(up, wHalf4_2, true) +
               PtrGlobalMatch(down, wHalf4_2, true) +

               PtrGlobalMatch(rows, wHalf4_3) +
               PtrGlobalMatch(columns, wHalf4_3) +
               PtrGlobalMatch(up, wHalf4_3, true) +
               PtrGlobalMatch(down, wHalf4_3, true) +

               PtrGlobalMatch(rows, wHalf4_4) +
               PtrGlobalMatch(columns, wHalf4_4) +
               PtrGlobalMatch(up, wHalf4_4, true) +
               PtrGlobalMatch(down, wHalf4_4, true) +

               PtrGlobalMatch(rows, wHalf4_5) +
               PtrGlobalMatch(columns, wHalf4_5) +
               PtrGlobalMatch(up, wHalf4_5, true) +
               PtrGlobalMatch(down, wHalf4_5, true) +

               PtrGlobalMatch(rows, wHalf4_6, false, END) +
               PtrGlobalMatch(columns, wHalf4_6, false, END) +
               PtrGlobalMatch(up, wHalf4_6, true, END) +
               PtrGlobalMatch(down, wHalf4_6, true, END) +

               PtrGlobalMatch(rows, wHalf4_7, false, BEGIN) +
               PtrGlobalMatch(columns, wHalf4_7, false, BEGIN) +
               PtrGlobalMatch(up, wHalf4_7, true, BEGIN) +
               PtrGlobalMatch(down, wHalf4_7, true, BEGIN);

    }
    else
    {
        return PtrGlobalMatch(rows, bHalf4_1) +
               PtrGlobalMatch(columns, bHalf4_1) +
               PtrGlobalMatch(up, bHalf4_1, true) +
               PtrGlobalMatch(down, bHalf4_1, true) +

               PtrGlobalMatch(rows, bHalf4_2) +
               PtrGlobalMatch(columns, bHalf4_2) +
               PtrGlobalMatch(up, bHalf4_2, true) +
               PtrGlobalMatch(down, bHalf4_2, true) +

               PtrGlobalMatch(rows, bHalf4_3) +
               PtrGlobalMatch(columns, bHalf4_3) +
               PtrGlobalMatch(up, bHalf4_3, true) +
               PtrGlobalMatch(down, bHalf4_3, true) +

               PtrGlobalMatch(rows, bHalf4_4) +
               PtrGlobalMatch(columns, bHalf4_4) +
               PtrGlobalMatch(up, bHalf4_4, true) +
               PtrGlobalMatch(down, bHalf4_4, true) +

               PtrGlobalMatch(rows, bHalf4_5) +
               PtrGlobalMatch(columns, bHalf4_5) +
               PtrGlobalMatch(up, bHalf4_5, true) +
               PtrGlobalMatch(down, bHalf4_5, true) +

               PtrGlobalMatch(rows, bHalf4_6, false, END) +
               PtrGlobalMatch(columns, bHalf4_6, false, END) +
               PtrGlobalMatch(up, bHalf4_6, true, END) +
               PtrGlobalMatch(down, bHalf4_6, true, END) +

               PtrGlobalMatch(rows, bHalf4_7, false, BEGIN) +
               PtrGlobalMatch(columns, bHalf4_7, false, BEGIN) +
               PtrGlobalMatch(up, bHalf4_7, true, BEGIN) +
               PtrGlobalMatch(down, bHalf4_7, true, BEGIN);
    }
}

int Board::PtrMatchHalf3(Board::Color color) const {
    if (color == WHITE)
    {
        return PtrGlobalMatch(rows, wHalf3_1) +
               PtrGlobalMatch(columns, wHalf3_1) +
               PtrGlobalMatch(up, wHalf3_1, true) +
               PtrGlobalMatch(down, wHalf3_1, true) +

               PtrGlobalMatch(rows, wHalf3_2) +
               PtrGlobalMatch(columns, wHalf3_2) +
               PtrGlobalMatch(up, wHalf3_2, true) +
               PtrGlobalMatch(down, wHalf3_2, true) +

               PtrGlobalMatch(rows, wHalf3_3) +
               PtrGlobalMatch(columns, wHalf3_3) +
               PtrGlobalMatch(up, wHalf3_3, true) +
               PtrGlobalMatch(down, wHalf3_3, true) +

               PtrGlobalMatch(rows, wHalf3_4) +
               PtrGlobalMatch(columns, wHalf3_4) +
               PtrGlobalMatch(up, wHalf3_4, true) +
               PtrGlobalMatch(down, wHalf3_4, true) +

               PtrGlobalMatch(rows, wHalf3_5) +
               PtrGlobalMatch(columns, wHalf3_5) +
               PtrGlobalMatch(up, wHalf3_5, true) +
               PtrGlobalMatch(down, wHalf3_5, true) +

               PtrGlobalMatch(rows, wHalf3_6) +
               PtrGlobalMatch(columns, wHalf3_6) +
               PtrGlobalMatch(up, wHalf3_6, true) +
               PtrGlobalMatch(down, wHalf3_6, true);
    }
    else
    {
        return PtrGlobalMatch(rows, bHalf3_1) +
               PtrGlobalMatch(columns, bHalf3_1) +
               PtrGlobalMatch(up, bHalf3_1, true) +
               PtrGlobalMatch(down, bHalf3_1, true) +

               PtrGlobalMatch(rows, bHalf3_2) +
               PtrGlobalMatch(columns, bHalf3_2) +
               PtrGlobalMatch(up, bHalf3_2, true) +
               PtrGlobalMatch(down, bHalf3_2, true) +

               PtrGlobalMatch(rows, bHalf3_3) +
               PtrGlobalMatch(columns, bHalf3_3) +
               PtrGlobalMatch(up, bHalf3_3, true) +
               PtrGlobalMatch(down, bHalf3_3, true) +

               PtrGlobalMatch(rows, bHalf3_4) +
               PtrGlobalMatch(columns, bHalf3_4) +
               PtrGlobalMatch(up, bHalf3_4, true) +
               PtrGlobalMatch(down, bHalf3_4, true) +

               PtrGlobalMatch(rows, bHalf3_5) +
               PtrGlobalMatch(columns, bHalf3_5) +
               PtrGlobalMatch(up, bHalf3_5, true) +
               PtrGlobalMatch(down, bHalf3_5, true) +

               PtrGlobalMatch(rows, bHalf3_6) +
               PtrGlobalMatch(columns, bHalf3_6) +
               PtrGlobalMatch(up, bHalf3_6, true) +
               PtrGlobalMatch(down, bHalf3_6, true);
    }
}

bool Board::PtrMatch(const Ptr &ptr) const {
    if (PtrGlobalMatch(rows, ptr))
        return true;
    else if (PtrGlobalMatch(columns, ptr))
        return true;
    else if (PtrGlobalMatch(up, ptr, true))
        return true;
    else if (PtrGlobalMatch(down, ptr, true))
        return true;
    else
        return false;
}

int Board::Eval() const {
    // Game ended
    switch (result) {
        case WHITE_WIN:
            return +100;
        case BLACK_WIN:
            return -100;
        case DRAW: // No available moves
            return 0;
        default:
            break;
    }
    if (PtrMatch(wFive))
        return +50;
    else if (PtrMatch(bFive))
        return -50;
    else if (PtrMatch(wZebra) && move == WHITE)
        return +30;
    else if (PtrMatch(bZebra) && move == BLACK)
        return -30;
    int evalScore{0};

    auto half4sW = PtrMatchHalf4(WHITE);
    auto half4sB = PtrMatchHalf4(BLACK);
    // Free 4 what is win or flanked four and your turn what is almost win
    if (PtrMatch(wFree4) || (half4sW && move == WHITE))
        return +40;
    else if (PtrMatch(bFree4) || (half4sB && move == BLACK))
        return -40;
    else if (half4sW > 1)
        return +35;
    else if (half4sB > 1)
        return -35;
    evalScore += (half4sW - half4sB) * 2;

    auto free3sW = PtrMatchFree3(WHITE);
    auto free3sB = PtrMatchFree3(BLACK);
    // Free 3 what is win or flanked four and your turn what is almost win
    if (free3sW && move == WHITE)
        return +15;
    else if (free3sB && move == BLACK)
        return -15;
    else if (free3sW > 1 && !free3sB)
        return +13;
    else if (free3sB > 1 && !free3sW)
        return -13;
    evalScore += (free3sW - free3sB) * 2;

    // Half 3 what is not a win but counts or flanked four and your turn what is not almost win
    auto half3sW = PtrMatchHalf3(WHITE);
    auto half3sB = PtrMatchHalf3(BLACK);
    evalScore += (half3sW - half3sB);

    // Future captures for available moves // TODO cW - cB * 4

    evalScore += ((int)white_captures_count - (int)black_captures_count) * 7;
    return (evalScore);
}
