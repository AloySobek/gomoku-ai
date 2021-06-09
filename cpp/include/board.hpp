#ifndef BOARD_HPP
# define BOARD_HPP

# include <iostream>
# include <cstdint>
# include <string>
# include <bitset>
# include <limits>

# define BOARD_SIZE 19

# define EMPTY_STONE 0
# define BLACK_STONE 1
# define WHITE_STONE 2

class Board
{
public:
    Board()
    {
        move_map[BOARD_SIZE / 2 * BOARD_SIZE + BOARD_SIZE / 2] = 1;
    }

    int8_t get_stone(int8_t x, int8_t y);

    bool place_stone_on_board(int8_t x, int8_t y, bool is_black, int8_t *captures=nullptr);
    bool remove_stone_from_board(int8_t x, int8_t y, bool is_black, int8_t *captures=nullptr); 

    bool five_in_a_row(bool is_black);
    int32_t eval(bool is_black);

    int32_t ai_move(bool is_black);
    int32_t minimax(int8_t depth, int32_t *alpha, int32_t *beta, bool maximizer, bool is_black);

    void reset();
    void print();

    int32_t *move_map{new int32_t[BOARD_SIZE * BOARD_SIZE]};
private:
    int32_t *black_board{new int32_t[BOARD_SIZE]};
    int32_t *white_board{new int32_t[BOARD_SIZE]};
};

#endif
