import sys

class Game:
    board_size = 19

    def __init__(self):
        self.board = [[0 for i in range(Game.board_size)] for j in range(Game.board_size)]

    def next_move(self):
        for y in range(Game.board_size):
            for x in range(Game.board_size):
                if self.board[y][x] == 0:
                    self.board[y][x] = 2
                    return
