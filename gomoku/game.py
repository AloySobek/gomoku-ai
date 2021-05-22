import sys

class Game:
    board_size = 19
    controller = None

    def __init__(self):
        self.board = [[0 for i in range(Game.board_size)] for j in range(Game.board_size)]

    def next_move(self):
        for y in range(Game.board_size):
            for x in range(Game.board_size):
                if self.board[y][x] == 0:
                    self.board[y][x] = 2
                    return

    def set(self, x, y, value):
        self.board[y][x] = value

    def is_valid_move(self, x, y, value):
        return self.board[y][x] == 0
