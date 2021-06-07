#include <iostream>
#include "gomoku.hpp"

int main()
{
    board_t *board = new board_t;

    board->move_map[9 * BOARD_SIZE + 9] = 1;

    /* place_stone_on_board(board,4,14,0); */
    /* place_stone_on_board(board,3,15,0); */
    /* place_stone_on_board(board,2,16,0); */
    /* place_stone_on_board(board,1,17,0); */
    /* place_stone_on_board(board,0,18,0); */
    /* place_stone_on_board(board,5,18,1); */

    /* place_stone_on_board(board,0,0,1); */
    /* place_stone_on_board(board,0,1,1); */
    /* place_stone_on_board(board,2,1,1); */
    /* place_stone_on_board(board,4,1,0); */
    /* remove_stone_from_board(board,4,1,0); */
    /* remove_stone_from_board(board,2,1,1); */
    /* remove_stone_from_board(board,0,1,1); */
    /* remove_stone_from_board(board,0,0,1); */

    ai_move(board);
    print_board(board);
    player_move(board,9,8);
    print_board(board);
    ai_move(board);
    print_board(board);

    return (0);
}
