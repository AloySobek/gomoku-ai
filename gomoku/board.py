""" This module contains stuff about Gomoku board """

from gomoku.vec2 import Vec2

class Board:
    """
    This class contains Gomoku board in several formats.
    Usual 2D array representing board as square  with 'size' side
    Binary format divided into two pieces for each player respectively
    """
    black_stone = 1
    white_stone = 2

    def __init__(self, size: int):
        self.size = size
        self.board = [[0 for _ in range(size)] for _ in range(size)]
        self.black_board = [0 for _ in range(size)] #  Binary representation of black player
        self.white_board = [0 for _ in range(size)] #  Binary representation of white player

    def place_stone(self, point: Vec2, is_black: bool):
        """
        This method places 'stone' on the board
        """
        if point.in_first_quadrant_square(self.size):
            if is_black:
                self.black_board[point.y] |= 262144 >> point.x
                self.board[point.y][point.x] = Board.black_stone
            else:
                self.white_board[point.y] |= 262144 >> point.x
                self.board[point.y][point.x] = Board.white_stone
        return self

    def remove_stone(self, point: Vec2, is_black: bool):
        """
        This method removes 'stone' from the board
        """
        if point.in_first_quadrant_square(self.size):
            if is_black:
                self.black_board[point.y] &= ~(262144 >> point.x)
            else:
                self.white_board[point.y] &= ~(262144 >> point.x)
            self.board[point.y][point.x] = 0
        return self

    def at(self, point: Vec2):
        """
        With this method you can get value from board with 2D Vector
        If something wrong - function returns -1
        """
        if point.in_first_quadrant_square(self.size):
            return self.board[point.y][point.x]
        return -1

    def __str__(self):
        string = ""
        for i in range(self.size):
            string += "{0:019b} {1:019b}\n".format(self.black_board[i], self.white_board[i])
        for i in range(self.size):
            string += "\t  "
            for j in range(self.size):
                string += str(self.board[i][j])
            string += "\n"
        return string
