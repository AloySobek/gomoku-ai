#ifndef GOMOKU_HPP
# define GOMOKU_HPP

# include <cstdint>
# include <cstdio>
# include <bitset>
# include <iostream>
# include <limits>
# include <cstdlib>
# include <ctime>

# define BOARD_SIZE 19

# define BLACK_STONE 1
# define WHITE_STONE 2
# define EMPTY 3
# define OUT_OF_RANGE -1

# define FIVE_IN_A_ROW_SCORE 100000

typedef struct  board_s
{
    int32_t     black_board[BOARD_SIZE];
    int32_t     white_board[BOARD_SIZE];
    int32_t     move_map[BOARD_SIZE * BOARD_SIZE];
}               board_t;

int8_t get_token(board_t *board, int16_t x, int16_t y);
int8_t place_stone_on_board(board_t *board, int16_t x, int16_t y, int8_t is_black);
int8_t remove_stone_from_board(board_t *board, int16_t x, int16_t y, int8_t is_black);

int32_t five_in_a_row(board_t *board);
int32_t open_four(board_t *board);
int32_t half_open_four(board_t *board);
int32_t open_three(board_t *board);
int32_t half_open_three(board_t *board);
int32_t open_two(board_t *board);

int32_t eval_board(board_t *board);
void print_board(board_t *board);
void ai_move(board_t *board);
int8_t player_move(board_t *board, int16_t x, int16_t y);
int32_t minimax(int8_t depth, int32_t *alpha, int32_t *beta, board_t *board, int8_t is_black);

#endif
