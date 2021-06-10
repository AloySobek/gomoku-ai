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

# define BOARD_SIZE 19

# define EMPTY_STONE 0
# define BLACK_STONE 1
# define WHITE_STONE 2

class Board
{
public:
    uint64_t score{0};

    uint64_t nodes_count{0};
    uint64_t pruned_count{0};
    uint64_t cache_hit_count{0};

    int32_t *move_map{new int32_t[BOARD_SIZE * BOARD_SIZE]()};
    int32_t *black_board{new int32_t[BOARD_SIZE]()};
    int32_t *white_board{new int32_t[BOARD_SIZE]()};

    Board();
    bool place_stone_on_board(int8_t x, int8_t y, bool is_black, uint8_t *captures=nullptr);
    bool remove_stone_from_board(int8_t x, int8_t y, bool is_black, uint8_t *captures=nullptr);
    int32_t minimax(int8_t depth, int32_t alpha, int32_t beta, bool maximizer, bool is_black);
    int32_t ai_move(bool is_black);
    void reset();
    void print();
    std::unordered_map<uint64_t, int32_t> hash_map;
private:
    uint64_t *zobrist_table{new uint64_t[BOARD_SIZE * BOARD_SIZE * 2]()};
    uint64_t hash{0};

    void fill_zobrist_table();
    uint64_t get_hash();
};

#endif
