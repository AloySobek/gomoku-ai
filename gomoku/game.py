from gomoku.ai import Ai
from gomoku.vec2 import Vec2
from gomoku.board import Board
from gomoku.board_analyzer import BoardAnalyzer

import sys

class Game:
    board_size = 19
    controller = None

    def __init__(self):
        self.ai = Ai()
        self.board = Board(Game.board_size)
        self.board_analyzer = BoardAnalyzer(Game.board_size)

    def ai_move(self):
        move = None
        max_h = float("-inf")
        for i in list(self.board_analyzer.possible_moves.keys()):
            if self.board.at(i) == 0:
                self.board.place_stone(i, True)
                self.board_analyzer.add_surroundings(self.board, i)
                h = self.ai.minimax(3, float("-inf"), float("inf"), self.board, self.board_analyzer, True)
                if h >= max_h:
                    max_h = h
                    move = i
                self.board_analyzer.remove_surroundings(self.board, i)
                self.board.remove_stone(i, True)
        self.board.place_stone(move, True)
        return move

    def player_move(self, point: Vec2):
        self.board.place_stone(point, False)
        self.board_analyzer.add_surroundings(self.board, point)
