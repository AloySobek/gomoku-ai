""" This module contains stuff about board analyzing"""

import random

from gomoku.vec2 import Vec2
from gomoku.board import Board

class BoardAnalyzer:
    # _FREE_PATTERNS = _genFreePatterns()
    # _PARTIAL_PATTERNS = _genPartialPatterns()
    # _CAPTURE_PATTERNS = _genCapturePatterns()
    # _WIN_PATTERNS = _getWinPatterns()

    five_in_a_row_score = 100000

    def __init__(self, size: int):
        self.size = size
        self.possible_moves = {Vec2(int(size/2),int(size/2)) : 1}

    def add_surroundings(self, board: Board, point: Vec2):
        #  TODO Memoize surroundings array
        surroundings = [
            point,
            Vec2(point.x,point.y-1),
            Vec2(point.x+1,point.y-1),
            Vec2(point.x+1,point.y),
            Vec2(point.x+1,point.y+1),
            Vec2(point.x,point.y+1),
            Vec2(point.x-1,point.y+1),
            Vec2(point.x-1,point.y),
            Vec2(point.x-1,point.y-1)
        ]
        for i in surroundings:
            if i.in_first_quadrant_square(board.size):
                if i in self.possible_moves:
                    self.possible_moves[i] += 1
                else:
                    self.possible_moves[i] = 1

    def remove_surroundings(self, board: Board, point: Vec2):
        #  TODO Memoize surroundings array
        surroundings = [
            point,
            Vec2(point.x,point.y-1),
            Vec2(point.x+1,point.y-1),
            Vec2(point.x+1,point.y),
            Vec2(point.x+1,point.y+1),
            Vec2(point.x,point.y+1),
            Vec2(point.x-1,point.y+1),
            Vec2(point.x-1,point.y),
            Vec2(point.x-1,point.y-1)
        ]
        for i in surroundings:
            if i.in_first_quadrant_square(board.size):
                if self.possible_moves[i] - 1 == 0:
                    self.possible_moves.pop(i)
                else:
                    self.possible_moves[i] -= 1

    def five_in_a_row(self, board: Board, is_black: bool):
        b = board.black_board if is_black else board.white_board
        for i in range(board.size):
            pattern = 0b11111
            for _ in range(14):
                if (b[i] & pattern) == pattern:
                    return self.five_in_a_row_score * (1 if is_black else -1)
                pattern <<= 1
            if i+4 < board.size:
                if b[i] & b[i+1] & b[i+2] & b[i+3] & b[i+4]:
                    return self.five_in_a_row_score * (1 if is_black else -1)
                if b[i] & (b[i+1] << 1) & (b[i+2] << 2) & (b[i+3] << 3) & (b[i+4] << 4):
                    return self.five_in_a_row_score * (1 if is_black else -1)
        return 0

    # def isDoubleThree(board: list(list(int)), xy: Vec2, v: int) -> bool:
    #     return sum(
    #         (1 for p in (getFreePlacing(board, xy, a, v) for a in ALL_AXIS) if p and p[0] and p[1] == 3)
    #     ) == 2

    # def _genPartialPatterns():
    #     base = {
    #         2: ("0xx0",),
    #         3: ("0xxx0", "0xx0x0", "0x0xx0"),
    #         4: ("0xxxx0", "0xxx0x0",),# "0xx0xx0", "0x0xxx0"),
    #     }
    #     _free_patterns = {}
    #     for k, v in base.items():
    #         for pl in ALL_P:
    #             _free_patterns[(k, pl)] = tuple(p.replace('x', f"{pl}") for p in v)
    #     for k, v in _free_patterns.items():
    #         _, pl = k
    #         if pl == P2:
    #             _free_patterns[k] = tuple(
    #                 list(map(lambda s: str(P1) + s[1:], v)) +
    #                 list(map(lambda s: s[:-1] + str(P1), v))
    #             )
    #         else:
    #             _free_patterns[k] = tuple(
    #                 list(map(lambda s: str(P2) + s[1:], v)) +
    #                 list(map(lambda s: s[:-1] + str(P2), v))
    #             )
    #         _free_patterns[k] = tuple(
    #             list(_free_patterns[k]) +
    #             list(map(lambda s: "[" + s[1:], _free_patterns[k])) +
    #             list(map(lambda s: s[:-1] + ']', _free_patterns[k]))
    #         )

    #     return _free_patterns

    # def _genFreePatterns():
    #     base = {
    #         2: ("0xx0",),
    #         3: ("0xxx0", "0xx0x0", "0x0xx0"),
    #         4: ("0xxxx0",),# "0xxx0x0", "0xx0xx0", "0x0xxx0"),
    #     }
    #     _free_patterns = {}
    #     for k, v in base.items():
    #         for pl in ALL_P:
    #             _free_patterns[(k, pl)] = tuple(p.replace('x', f"{pl}") for p in v)
    #     return _free_patterns

    # def _genCapturePatterns():
    #     return {
    #         P1: ("1221",),
    #         P2: ("2112",)
    #     }

    # def _getWinPatterns():
    #     return {
    #         P1: "11111",
    #         P2: "22222"
    #     }

    # def _getFlat(board: List[List[int]], xy: Vec2, dxy: Vec2, v: int, extent: int) -> str:
    #     """Flattens forward diagonal / row / column / backward diagonal for xy

    #     1..
    #     .2.  for xy (1, 1) dxy (1, 1) and extent 2 forms string .121.     from forward diagonal
    #     ..1  for xy (2, 2) dxy (1, 1) and extent 3 forms string .121...   from forward diagonal
    #          for xy (1, 1) dxy (1, 0) and extent 4 forms string ....2.... from row

    #     @TODO: We can cache coordinates for each xy for each dxy in to the dict to optimize
    #     """
    #     def get(_xy):
    #         if (_xy.x < 0 or _xy.y < 0):
    #             raise IndexError
    #         return board[_xy.y][_xy.x]

    #     flat = f"{v}"
    #     for i in range(1, extent):
    #         try:
    #             flat = str(get(xy.move(dxy, -i))) + flat
    #         except IndexError:
    #             if flat[0] not in '[.':
    #                 flat = "[" + flat
    #             else:
    #                 flat = '.' + flat

    #         try:
    #             flat += str(get(xy.move(dxy, +i)))
    #         except IndexError:
    #             if flat[-1] not in '].':
    #                 flat += ']'
    #             else:
    #                 flat += '.'

    #     # eprint('b:', '\n'.join(map(lambda r: "".join(map(str, r)), board)))
    #     # eprint(flat)
    #     return flat

    def eval_board(self, board: Board, is_black: bool):
        score = random.randint(0, 100000)
        # Pattern sequence right here
        return score * (1 if is_black else -1)
