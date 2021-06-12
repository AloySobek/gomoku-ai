#ifndef BOARD_HPP
# define BOARD_HPP

# include <iostream>
# include <cstdint>
# include <string>
# include <bitset>
# include <limits>
# include <chrono>
# include <random>
# include <unordered_map>
#include <vector>

# define BOARD_SIZE 19

# define EMPTY_STONE 0
# define BLACK_STONE 1
# define WHITE_STONE 2

class Board
{
public:
    uint64_t nodes_count{0};
    uint64_t pruned_count{0};
    uint64_t cache_hit_count{0};

    uint64_t black_captures_count{0};
    uint64_t white_captures_count{0};

    int32_t *black_board{new int32_t[BOARD_SIZE]()};
    int32_t *white_board{new int32_t[BOARD_SIZE]()};
    int32_t *move_map{new int32_t[BOARD_SIZE * BOARD_SIZE]()};

    Board();
    bool place_stone_on_board(int32_t x, int32_t y, bool is_black, int32_t *captures=nullptr);
    bool remove_stone_from_board(int32_t x, int32_t y, bool is_black, int32_t *captures=nullptr);
    int32_t minimax(int32_t depth, int32_t alpha, int32_t beta, int32_t x, int32_t y, bool maximizer, bool is_black);
    int32_t ai_move(bool is_black);
    void reset();
    void print();

private:
    uint64_t *zobrist_table{new uint64_t[BOARD_SIZE * BOARD_SIZE * 2]()};
    std::unordered_map<uint64_t, int32_t> hash_map;
    uint64_t hash{0};

    int32_t start_x{0}, start_y{0};
    int32_t end_x{0}, end_y{0};
    int32_t most_left, most_right{0};

    void fill_zobrist_table();
    uint64_t get_hash();

    bool five_in_a_row(int32_t x, int32_t y, bool is_black);
    bool open_four(int32_t x, int32_t y, bool is_black);
    bool simple_four(int32_t x, int32_t y, bool is_black);
    bool open_three(int32_t x, int32_t y, bool is_black);
    bool simple_three(int32_t x, int32_t y, bool is_black);
    bool open_two(int32_t x, int32_t y, bool is_black);
    bool simple_two(int32_t x, int32_t y, bool is_black);
};

#endif
